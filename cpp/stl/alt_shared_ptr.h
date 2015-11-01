#ifndef ALT_SHARED_PTR_H
#define ALT_SHARED_PTR_H

#include <iostream>
#include <utility>
#include <cassert>

template <class T>
void default_delete(T *p)
{
        delete p;
}

template <class T>
void default_array_delete(T *p)
{
        delete [] p;
}

template <class T>
class shared_ptr
{
    using pointer = T*;
    using reference = T&;
    using delete_fn = void (*) (pointer);
    using counter = int;

    pointer ptr;
    counter *cnt;
    delete_fn del;

public:
    shared_ptr() noexcept
            : ptr(nullptr), cnt(nullptr), del( default_delete<T> )
    {
            std::cout << "shared_ptr()\n";
    }

    explicit shared_ptr(pointer p) noexcept
            : ptr(p), cnt( new counter(1) ), del( default_delete<T> )
    {
            std::cout << "shared_ptr(pointer)\n";
    }

    shared_ptr(pointer p, delete_fn d) noexcept
            : ptr(p), cnt( new counter(1) ), del(d)
    {
            std::cout << "shared_ptr(pointer, delete_fn)\n";
    }

    shared_ptr(const shared_ptr &x) noexcept
            : shared_ptr(x.ptr, x.del)
    {
            cnt = x.cnt;
            ++*cnt;
            std::cout << "shared_ptr(const shared_ptr&)\n";
    }

    shared_ptr(shared_ptr &&x)
            : ptr( std::move(x.ptr) ),
              cnt( std::move(x.cnt) ),
              del( std::move(x.del) )
    {
            std::cout << "shared_ptr(shared_ptr&&)\n";
            x.ptr = nullptr;
            x.cnt = nullptr;
    }

    shared_ptr& operator = (const shared_ptr &x)
    {
            std::cout << "shared_ptr::operator=&\n";
            if (this != &x) {
                    reset();
                    ptr = x.ptr;
                    cnt = x.cnt;
                    del = x.del;
                    ++*cnt;
            }
            return *this;
    }

    shared_ptr& operator = (shared_ptr &&x)
    {
            std::cout << "shared_ptr::operator=&&\n";
            shared_ptr::swap(x);
            return *this;
    }

    void swap(shared_ptr &x)
    {
            std::cout << "shared_ptr::swap(shared_ptr&)\n";
            std::swap(ptr, x.ptr);
            std::swap(cnt, x.cnt);
            std::swap(del, x.del);
    }

    ~shared_ptr()
    {
            std::cout << "~shared_ptr::~shared_ptr()\n";
            if (cnt != nullptr && --*cnt == 0) {
                    std::cout << " destroyed " << *ptr << "\n";
                    del(ptr);
                    delete cnt;
            }
    }

    pointer operator -> () const noexcept
    {
            return ptr;
    }

    reference operator * () const noexcept
    {
            return *ptr;
    }

    pointer get() const noexcept
    {
            return ptr;
    }

    explicit operator bool() const noexcept
    {
            return ptr != nullptr;
    }

    bool unique() const noexcept
    {
            return use_count() == 1;
    }

    counter use_count() const noexcept
    {
            return cnt != nullptr ? *cnt : 0;
    }

    void reset()
    {
            this->~shared_ptr();
            ptr = nullptr;
            cnt = nullptr;
    }

    void reset(pointer p)
    {
            reset();
            cnt = new counter(1);
            ptr = p;
    }

    void reset(pointer p, delete_fn d)
    {
            reset();
            cnt = new counter(1);
            ptr = p;
            del = d;
    }
};

template <class T, class... Args>
shared_ptr<T> make_shared(Args&& ...args)
{
        return shared_ptr<T>( new T( std::forward<Args>(args)... ) );
}

#endif // ALT_SHARED_PTR_H
