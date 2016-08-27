#ifndef REF_PTR_H
#define REF_PTR_H

#include <iostream>
#include <list>
#include <thread>

template<typename T>
class ref_ptr
{
    using pointer = T *;
    using reference = T &;
    using counter = std::size_t;

public:
    // Default constructor
    ref_ptr(void) noexcept
        : t {nullptr}, is {nullptr}, object_ptr {nullptr}
    {
        t = new T {};
        //*t = 0;
        is = new bool{ true };
        object_ptr = new std::list<ref_ptr *>;
    }

    // Constructor for pointer
    explicit ref_ptr(T *ptr) noexcept
        : t{nullptr}, is{nullptr}, object_ptr{nullptr}
    {
        t = ptr;
        is = new bool{ true };
        object_ptr = new std::list<ref_ptr *>;
    }

    // Copy constructor
    ref_ptr(ref_ptr const & ref) noexcept
        : t{nullptr}, is{nullptr}, object_ptr{nullptr}
    {
        #if !defined(NDEBUG)
            std::cout << "Copy to root object" << std::endl;
        #endif
        t = ref.t;
        is = new bool{ true };
        object_ptr = ref.object_ptr;
        //++*root;
    }

    // Move constructor
    ref_ptr(ref_ptr && xref)
            : t( std::move(xref.t) ),
              root( std::move(xref.root),
              object_ptr( std::move(xref.object_ptr)))
    {
        #if !defined(NDEBUG)
            std::cout << "Move object" << std::endl;
        #endif
        xref.t = nullptr;
        xref.root = nullptr;
        xref.object_ptr = nullptr;
    }

    // Operator = for reference
    ref_ptr & operator = (ref_ptr & ref)
    {
        if(this != &ref && t != ref.t)
        {
            reset();
            #if !defined(NDEBUG)
                std::cout << "Copy object" << std::endl;
                std::cout << "  ptr.objects = " << ref.object_ptr->size() << std::endl;
                //std::cout << "  ptr.value = " << *ref.t << std::endl;
            #endif
            t = ref.t;
            is = ref.is;
            object_ptr = ref.object_ptr;
            object_ptr->push_back(this);
        }
        return *this;
    }

    // Operator = for xreference
    ref_ptr & operator = (ref_ptr && xref)
    {
        #if !defined(NDEBUG)
            std::cout << "Move object xref to this" << std::endl;
            std::cout << "  ptr.root = " << *xref.root << std::endl;
            std::cout << "  ptr.objects = " << xref.object_ptr->size() << std::endl;
            std::cout << "  ptr.value = " << *xref.t << std::endl;
        #endif
        ref_ptr::swap(xref);
        return *this;
    }

    // Swap reference
    void swap(ref_ptr &ref)
    {
        #if !defined(NDEBUG)
            std::cout << "Swap objects" << std::endl;
        #endif
        std::swap(t, ref.t);
        std::swap(root, ref.root);
        std::swap(object_ptr, ref.object_ptr);
    }

    T & operator *() const noexcept
    {
        return *t;
    }

    T * operator ->() const noexcept
    {
        return t;
    }

    explicit operator bool() const noexcept
    {
        return t != nullptr;
    }

    T & operator = (T & ref)
    {
        if(t != &ref)
        {
            *t = ref;
        }
        return *t;
    }

    ~ref_ptr()
    {
      *is = false;
      *root -= 1;
      if (*root > 0)
      {
         t->~T();
      }
      else
      {
         delete t;
      }
    }

    void reset(void)
    {
        this->~ref_ptr();
        t = nullptr;
        *is = false;
        object_ptr = nullptr;
    }

    void reset(T *ptr)
    {
        reset();
        t = ptr;
        is = new bool{ true };
        object_ptr = new std::list<ref_ptr *>;
    }

private:
    T                       *t;
    bool                    *is;
    int                     *root;
    std::list<ref_ptr *>    *object_ptr;
};

#endif // REF_PTR_H
