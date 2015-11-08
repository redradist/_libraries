#ifndef EVENT_H
#define EVENT_H

#include "observer.h"
#include "mutex"
#include "vector"

namespace evt 
{
   using namespace std;
   using namespace observer;

   template <typename Type>
   class Event;

   template <typename Return, typename... Args>
   class Event<Return(*)(Args...)>
   {
   public:
      using Observer = Observer<Return, Args...>;
      using Func = Return(*)(Args...);
      
      class CallBack : public Observer
      {
      private:
         Func   mFunc; // Address of the function on the delegate object.

      public:
         CallBack(Func aFunc)
            : mFunc(aFunc)  { }

         virtual Return operator()(Args... args) const
         {
            return (*mFunc)(args...);
         }
      }; // Static

      template <typename Class>
      class Listener : public Observer
      {
         using Meth = Return(Class::*)(Args...);

         Class  *mCaller;
         Meth   mMeth; // Address of the function on the delegate object.

      public:
         Listener(Class *aCaller, Meth aMeth)
            : mCaller(aCaller)
            , mMeth(aMeth) { }

         virtual Return operator()(Args... args) const
         {
            return (mCaller->*mMeth)(args...);
         }
      }; // Non-static

      Event() { }

      Event(Func aFunc)
      {
         *this += new Listener(aFunc);
      }

      Event(Observer* aPtr)
      {
         if (aPtr != nullptr)
         {
            mPtrs.push_back(aPtr);
         }
      }

      // Add a new target (callee) to our list.
      Event& operator +=(Observer* aPtr)
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
         typename std::vector<Observer*>::iterator end = mPtrs.end();
         for (typename std::vector<Observer*>::iterator i = mPtrs.begin(); i != end; ++i)
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
      std::vector<Observer*>      mPtrs;
   };

   template <typename Func>
   Event<Func> make_event(Func func)
   {
      return Event<Func>(func);
   }

   template <typename Func>
   typename Event<Func>::CallBack make_callback(Func func)
   {
      return typename Event<Func>::CallBack(func);
   }
}

#endif // EVENT_H
