#ifndef EVENT_H
#define EVENT_H

#include "observer.h"
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
      using Observer = observer::Observer<Return, Args...>;
      using Func = Return(*)(Args...);
      
      class CallBack : public Observer
      {
      private:
         Func   mFunc; // Address of the function on the delegate object.

      public:
         CallBack(Func aFunc)
            : mFunc(aFunc)  { }

         virtual inline Return operator()(Args... args) const
         {
            return (*mFunc)(args...);
         }
      }; // Static
      
      class Listener : public Observer
      {
         class IContainer { public: virtual Return call(Args...) = 0; };
         
         template<typename Class>
         class Container : public IContainer
         {
            using Meth = Return(Class::*)(Args...);
         public: 
            Container(Class* aObj, Meth aMeth)
               : mObj(aObj)
               , mMeth(aMeth) { }
         private: 
            Class    *mObj; 
            Meth     mMeth;
         public: 
            Return call(Args... args)
            {
               return (mObj->*mMeth)(args...);
            }
         };

         IContainer  *iContainer;

      public:
         template <typename Class>
         Listener(Class *aCaller, Return(Class::*aMeth)(Args...))
            : iContainer(nullptr)
         {
            if (iContainer != nullptr) delete iContainer;
            iContainer = new Container<Class>(aCaller, aMeth);
         }
         
         virtual ~Listener()
         {
            if (iContainer != nullptr) delete iContainer;
         }

         virtual inline Return operator()(Args... args) const
         {
            return iContainer->call(args...);
         }
      }; // Non-static

      Event() { }

      Event(Func aFunc)
      {
         *this += new CallBack(aFunc);
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

   template <typename Func>
   void add_callback(Event<Func> &evt, Func func)
   {
      evt += new Event<Func>::CallBack(func);
   }

   template <typename Func, typename Class, typename Meth>
   void add_listener(Event<Func> &evt, Class obj, Meth meth)
   {
      evt += new Event<Func>::Listener(&obj, meth);
   }
}

#endif // EVENT_H
