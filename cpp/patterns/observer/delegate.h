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

   template <typename Class, typename Return, typename... Args>
   class Delegate<Return(Class::*)(Args...)> : Observer<Return, Args...>
   {
      using Meth = Return(Class::*)(Args...);

   private:
      recursive_mutex      *mMutex;
      Class                &mCaller;
      Meth                 mMeth;

   public:
      Delegate() { }

      Delegate(Class &aCaller, Meth aMeth)
         : mCaller(aCaller)
         , mMeth(aMeth)
         , mMutex(new recursive_mutex()) { }

      ~Delegate() { delete mMutex; }

      virtual Return operator()(Args... args) const
      {
         mMutex->lock();
         Return result = (mCaller.*mMeth)(args...);
         mMutex->unlock();
         return result;
      }

      operator Observer *()
      {
         return this;
      }
   };

   template <typename Class, typename... Args>
   class Delegate<void(Class::*)(Args...)> : Observer<void, Args...>
   {
   public:
      using Meth = void(Class::*)(Args...);

      Delegate() { }

      Delegate(Class &aCaller, Meth aMeth)
         : mCaller(aCaller)
         , mMeth(aMeth)
         , mMutex(new recursive_mutex()) { }

      ~Delegate() { delete mMutex; }

      virtual void operator()(Args... args) const
      {
         mMutex->lock();
         (mCaller.*mMeth)(args...);
         mMutex->unlock();
         return;
      }

   private:
      recursive_mutex      *mMutex;
      Class                &mCaller;
      Meth                 mMeth;
   };

   template <typename Return, typename... Args>
   class Delegate<Return(*)(Args...)> : Observer<Return, Args...>
   {
   public:
      using Func = Return(*)(Args...);

      Delegate() { }

      Delegate(Func aFunc)
         : mFunc(aFunc)
         , mMutex(new recursive_mutex()) { }

      ~Delegate() { delete mMutex; }

      virtual Return operator()(Args... args) const
      {
         mMutex->lock();
         Return result = (*mFunc)(args...);
         mMutex->unlock();
         return result;
      }

      /*operator Observer()
      {
         return &this;
      }*/

   private:
      recursive_mutex      *mMutex;
      Func                 mFunc;
   };

   template <typename... Args>
   class Delegate<void(*)(Args...)> : Observer<void, Args...>
   {
   public:
      using Func = void(*)(Args...);

      Delegate() { }

      Delegate(Func aFunc)
         : mFunc(aFunc)
         , mMutex(new recursive_mutex()) { }

      ~Delegate() { delete mMutex; }

      virtual void operator()(Args... args) const
      {
         mMutex->lock();
         (*mFunc)(args...);
         mMutex->unlock();
         return;
      }

   private:
      recursive_mutex      *mMutex;
      Func                 mFunc;
   };

   template <typename Func>
   Delegate<Func> make_delegate(Func func)
   {
      return Delegate<Func>(func);
   }

   template <typename Class, typename Func>
   Delegate<Func> make_delegate(Class &obj, Func func)
   {
      return Delegate<Func>(obj, func);
   }

   /*template <typename Class, typename Func>
   typename Observer *add_delegate(Delegate<Func> &obj)
   {
      return reinterpret_cast<typename Observer*> (&obj);
   }*/
}

#endif // DELEGATE_H