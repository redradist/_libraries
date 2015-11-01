#ifndef REF_PTR_H
#define REF_PTR_H

#include <iostream>
#include <list>
#include <thread>

template<typename T>
class ref_ptr
{
    T   *t;
public:
    ref_ptr(void) noexcept
        : t{nullptr}, root{nullptr}, object_ptr{nullptr}
    {
        t = new T();
        *t = 0;
        root = new int {1};
        object_ptr = new std::list<ref_ptr *>;
    }

    explicit ref_ptr(T *ptr) noexcept
        : t{nullptr}, root{nullptr}, object_ptr{nullptr}
    {
        t = ptr;
        root = new int {1};
        object_ptr = new std::list<ref_ptr *>;
    }

    ref_ptr(ref_ptr & ref)
        : t{nullptr}, root{nullptr}, object_ptr{nullptr}
    {
        #if !defined(NDEBUG)
            std::cout << "Copy to root object" << std::endl;
        #endif
        root = ref.root;
        object_ptr = ref.object_ptr;
        ++*root;
        t = ref.t;
    }

    ref_ptr & operator = (ref_ptr & ref)
    {
        if(this != &ref && *t != *ref.t)
        {
            reset();
            #if !defined(NDEBUG)
                std::cout << "Copy object" << std::endl;
                std::cout << "  ptr.root = " << *ref.root << std::endl;
                std::cout << "  ptr.objects = " << ref.object_ptr->size() << std::endl;
                std::cout << "  ptr.value = " << *ref.t << std::endl;
            #endif
            root = ref.root;
            object_ptr = ref.object_ptr;
            object_ptr->push_back(&ref);
            t = ref.t;
        }
        else
        {

        }
        return *this;
    }

    /*ref_ptr & operator = (ref_ptr && xref)
    {
        #if !defined(NDEBUG)
            std::cout << "shared_ptr::operator=&&\n";
        #endif
        ref_ptr::swap(xref);
        return *this;
    }

    void swap(ref_ptr &ref)
    {
        #if !defined(NDEBUG)
            std::cout << "shared_ptr::swap(shared_ptr&)\n";
        #endif
        std::swap(root, ref.root);
        std::swap(object_ptr, ref.object_ptr);
    }*/

    T operator*() const noexcept
    {
        return *t;
    }

    T operator->() const noexcept
    {
        return t;
    }

    explicit operator bool() const noexcept
    {
            return t != nullptr;
    }



    T & operator = (T & ref)
    {
        if(*t != *ref)
        {
            *t = ref;
        }
        return *t;
    }

    ~ref_ptr()
    {
        #if !defined(NDEBUG)
            std::cout << "Delete root object" << std::endl;
            std::cout << "  root = " << *root << std::endl;
            std::cout << "  objects = " << object_ptr->size() << std::endl;
        #endif
        if(root != nullptr && object_ptr != nullptr)
        {
            if(*root > 0)
            {
                --*root;
            }
            if(!object_ptr->empty())
            {
                // Checking if this is me
                for(auto item = object_ptr->begin();item != object_ptr->end(); ++item)
                {
                    ref_ptr *ptr = *item;
                    if(this == ptr)
                    {
                        #if !defined(NDEBUG)
                            std::cout << "  object is this" << std::endl;
                        #endif
                        object_ptr->erase(item);
                        break;
                    }
                }
            }
            // If root object is deleted and object local is empty then delete
            if(*root == 0 && object_ptr->empty())
            {
                #if !defined(NDEBUG)
                    std::cout << "  delete real object" << std::endl;
                    std::cout << "    value is " << *t << std::endl;
                #endif
                delete root;
                root = nullptr;
                delete object_ptr;
                object_ptr = nullptr;
                delete t;
                t = nullptr;
            }
        }
    }

    void reset(void)
    {
        this->~ref_ptr();
        root = nullptr;
        object_ptr = nullptr;
    }

    void reset(T *ptr)
    {
        reset();
        root = new int{1};
        t = ptr;
    }

private:
    int *root;
    std::list<ref_ptr *> *object_ptr;
};

#endif // REF_PTR_H
