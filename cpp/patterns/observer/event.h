#ifndef EVENT_H
#define EVENT_H

#include "vector"
#include "delegate.h"

namespace evt 
{
   using namespace dlgt;

   template <typename Class>
   class Event;

   template <typename Return, typename... Args>
   class Event<Delegate<Return(*)(Args...)>>
   {
   private:

      class Base
      {
      public:
         virtual ~Base() { }
         virtual Return operator()(Args... args) = 0;
      };

      using Func = Return(*)(Args...);
      std::vector<Base*>      mPtrs;

   public:

      class Maker : public Base
      {
      private:
         Delegate&   mDlgt; // Address of the function on the delegate object.

      public:
         Maker(Delegate& aDlgt) : mDlgt(aDlgt) { }

         virtual Return operator()(Args... args)
         {
            return mDlgt(args...);
         }
      }; // Non-static

      Event() { }

      Event(Delegate& aDlgt)
      {
         *this += new Maker(aDlgt);
      }

      // Add a new target (callee) to our list.
      Event& operator +=(Base* aPtr)
      {
         if (aPtr != nullptr)
         {
            mPtrs.push_back(&aPtr);
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

   template <typename Class, typename Return, typename... Args>
   class Event<Delegate<Return(Class::*)(Args...)>>
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
         Delegate&   mDlgt; // Address of the function on the delegate object.

      public:
         Maker(Class* aThis, Delegate aDlgt) : mThis(aThis), mDlgt(aDlgt) { }

         virtual Return operator()(Args... args)
         {
            return (mThis->mDlgt)(args...);
         }
      }; // Non-static

      Event() { }

      Event(Class* aThis, Delegate aDlgt)
      {
         *this += new Maker(aThis, aDlgt);
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

   template <typename Dlg>
   Event<Dlg> make_event(Dlg dlg)
   {
      return Event<Dlg>(dlg);
   }

   template <typename Class, typename Dlg>
   Event<Dlg> make_event(Class& obj, Dlg dlg)
   {
      return Event<Dlg>(&obj, dlg);
   }
}

#endif // EVENT_H
