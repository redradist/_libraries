#ifndef EVENT_H
#define EVENT_H

#include "vector"
#include "delegate.h"



namespace evt 
{
   //using namespace dlgt;
   
   class BaseDelegate
   {
   public:
      virtual ~Delegate() { }
      virtual Return operator()(Args... args) = 0;
   };

   class Maker : public BaseDelegate
   {
   private:
      recursive_mutex   *mMutex;
      Func   mFunc; // Address of the function on the delegate object.

   public:
      Maker(Func aFunc)
         : mFunc(aFunc)
         , mMutex(new recursive_mutex()) { }

      virtual Return operator()(Args... args)
      {
         mMutex->lock();
         Return result = (*mFunc)(args...);
         mMutex->unlock();
         return result;
      }
   }; // Non-static

   template <typename Class>
   class Event;

   template <typename Return, typename... Args>
   class Event<Return(*)(Args...)>
   {
   public:
      using Func = Return(*)(Args...);

      class Delegate
      {
      public:
         virtual ~Delegate() { }
         virtual Return operator()(Args... args) = 0;
      };

      class Maker : public Delegate
      {
      private:
         recursive_mutex   *mMutex;
         Func   mFunc; // Address of the function on the delegate object.

      public:
         Maker(Func aFunc)
            : mFunc(aFunc)
            , mMutex(new recursive_mutex())  { }

         virtual Return operator()(Args... args)
         {
            mMutex->lock();
            Return result = (*mFunc)(args...);
            mMutex->unlock();
            return result;
         }
      }; // Non-static

      Event() { }

      Event(Func aFunc)
      {
         *this += new Maker(aFunc);
      }

      Event(Delegate* aPtr)
      {
         if (aPtr != nullptr)
         {
            mPtrs.push_back(aPtr);
         }
      }

      // Add a new target (callee) to our list.
      Event& operator +=(Delegate* aPtr)
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
         typename std::vector<Delegate*>::iterator end = mPtrs.end();
         for (typename std::vector<Delegate*>::iterator i = mPtrs.begin();
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

   private:
      std::vector<Delegate*>      mPtrs;
   };

   /*
   template <typename Class, typename Return, typename... Args>
   class Event<Return(Class::*)(Args...)>
   {
   private:

      class Base
      {
      public:
         virtual ~Base() { }
         virtual Return operator()(Args... args) = 0;
      };

      using Meth = Return(Class::*)(Args...);
      std::vector<Base*>      mPtrs;

   public:

      class Maker : public Base
      {
      private:
         Class* mThis; // Pointer to the object we are delegating to.
         Meth&   mMeth; // Address of the function on the delegate object.

      public:
         Maker(Class* aThis, Meth aMeth) : mThis(aThis), mMeth(aMeth) { }

         virtual Return operator()(Args... args)
         {
            return (mThis->*mDlgt)(args...);
         }
      }; // Non-static

      Event() { }

      Event(Class* aThis, Meth aMeth)
      {
         *this += new Maker(aThis, aMeth);
      }

      // Add a new target (callee) to our list.
      Event& operator +=(Base* aPtr)
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
   */

   template <typename Func>
   Event<Func> make_event(Func func)
   {
      return Event<Func>(func);
   }

   template <typename Class, typename Func>
   Event<Func> make_event(Class& obj, Func func)
   {
      return Event<Func>(&obj, func);
   }
}

#endif // EVENT_H
