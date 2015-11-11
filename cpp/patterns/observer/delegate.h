#ifndef DELEGATE_H
#define DELEGATE_H

#include "observer.h"
#include "mutex"

namespace dlgt
{
   using namespace std;
   using namespace observer;

   template <typename Type>
   class Delegate;

   template <typename Return, typename... Args>
   class Delegate<Return(*)(Args...)> : public Observer<Return, Args...>
   {
      using Func = Return(*)(Args...);
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

      template<>
      class Container<void> : public IContainer
      {
      public:
         Container(Func aFunc)
            : mFunc(aFunc) { }
      private:
         Func     mFunc;
      public:
         Return call(Args... args)
         {
            return (*mFunc)(args...);
         }
      };

      IContainer  *iContainer;

   public:
      Delegate() { }

      template <typename Class>
      Delegate(Class &aCaller, Return(Class::*aMeth)(Args...))
         : mMutex(new recursive_mutex()) 
      {
         if (iContainer != nullptr) delete iContainer;
         iContainer = new Container<Class>(&aCaller, aMeth);
      }

      Delegate(Func aFunc)
         : mMutex(new recursive_mutex()) 
      { 
         if (iContainer != nullptr) delete iContainer;
         iContainer = new Container<void>(aFunc);
      }

      ~Delegate() { delete mMutex; }

      virtual inline Return operator()(Args... args) const
      {
         if (iContainer != nullptr)
         {
            mMutex->lock();
            Return result = iContainer->call(args...);
            mMutex->unlock();
            return result;
         }
      }

   private:
      recursive_mutex      *mMutex;
   };

   template <typename... Args>
   class Delegate<void(*)(Args...)> : public Observer<void, Args...>
   {
      using Func = void(*)(Args...);
      class IContainer { public: virtual void call(Args...) = 0; };

      template<typename Class>
      class Container : public IContainer
      {
         using Meth = void(Class::*)(Args...);
      public:
         Container(Class* aObj, Meth aMeth)
            : mObj(aObj)
            , mMeth(aMeth) { }
      private:
         Class    *mObj;
         Meth     mMeth;
      public:
         void call(Args... args)
         {
            return (mObj->*mMeth)(args...);
         }
      };

      template<>
      class Container<void> : public IContainer
      {
      public:
         Container(Func aFunc)
            : mFunc(aFunc) { }
      private:
         Func     mFunc;
      public:
         void call(Args... args)
         {
            return (*mFunc)(args...);
         }
      };

      IContainer  *iContainer;

   public:
      Delegate() { }

      template <typename Class>
      Delegate(Class &aCaller, void(Class::*aMeth)(Args...))
         : mMutex(new recursive_mutex())
      {
         if (iContainer != nullptr) delete iContainer;
         iContainer = new Container<Class>(&aCaller, aMeth);
      }

      Delegate(Func aFunc)
         : mMutex(new recursive_mutex())
      {
         if (iContainer != nullptr) delete iContainer;
         iContainer = new Container<void>(aFunc);
      }

      ~Delegate() { delete mMutex; }

      virtual inline void operator()(Args... args) const
      {
         if (iContainer != nullptr)
         {
            mMutex->lock();
            iContainer->call(args...);
            mMutex->unlock();
         }
         return;
      }

   private:
      recursive_mutex      *mMutex;
   };

   template <typename Func>
   Delegate<Func> make_delegate(Func func)
   {
      return Delegate<Func>(func);
   }
}

#endif // DELEGATE_H
