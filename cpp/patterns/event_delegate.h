#ifndef EVENT_DELEGATE_H
#define EVENT_DELEGATE_H

#include "mutex"
#include "vector"

template <typename Return, typename... Args>
class Event
{
private:

   class Base
   {
   public:
      virtual ~Base() { }
      virtual Return operator()(Args...) = 0;
   };

   std::vector<Base*>      mPtrs;

public:
   template <typename Class>
   class T : public Base
   {
      // Signature applied to a pointer-to-member for target class.
      typedef Return(Class::*Func)(Args...);

   private:
      Class* mThis; // Pointer to the object we are delegating to.
      Func   mFunc; // Address of the function on the delegate object.

   public:
      T(Class* aThis, Func aFunc) : mThis(aThis), mFunc(aFunc) { }

      virtual Return operator()(Args... args)
      {
         return (mThis->*mFunc)(args...);
      }
   }; // Non-static

   class S : public Base
   {
      typedef Return(*Func)(Args...);

   private:
      Func mFunc;

   public:
      S(Func aFunc) : mFunc(aFunc) { }

      virtual Return operator()(Args... args)
      {
         return mFunc(args...);
      }
   }; // Static
                                  // Add a new target (callee) to our list.
   Event& operator+=(Base* aPtr)
   {
      if (aPtr != nullptr)
      {
         mPtrs.push_back(aPtr);
      }
      return *this;
   }

   // Call all the targets - there will be horrible undefined behaviour
   // if the callee object no longer exists. You have been warned!
   Return operator()(Args... args)
   {
      // There are problems here:
      // 1. Which result should the multicast return? 
      // For now I say the last called item.
      // 2. We need to can't store a temporary when Return is void.
      typename std::vector<Base*>::iterator end = mPtrs.end();
      for (typename std::vector<Base*>::iterator i = mPtrs.begin();
      i != end; ++i)
      {
         // Probably a specialisation for Return == void would be better.
         if ((i + 1) == end)
         {
            return (*(*i))(args...);
         }
         else
         { 
            (*(*i))(args...);
         }
      }
   }
};

template <typename F, typename T>
Event<F> make_event(F func, T& obj)
{
   return Event<F>(func, obj);
}

template <typename T>
Event<T> make_event(T func)
{
   return Event<T>(func);
}

#endif // EVENT_DELEGATE_H
