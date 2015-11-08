#ifndef DLGT_DELEGATE_H_
#define DLGT_DELEGATE_H_

#include "mutex"

namespace dlgt
{
   using namespace std;
   
   /**
   * non specialized template declaration for delegate
   */
   template <typename Class>
   class Delegate;

   /**
   * specialization for member functions
   *
   * \tparam T            class-type of the object who's member function to call
   * \tparam R            return type of the function that gets captured
   * \tparam params       variadic template list for possible arguments
   *                      of the captured function
   */
   template <typename Class, typename Return, typename... Args>
   class Delegate<Return(Class::*)(Args...)>
   {
   public:
      using Func = Return(Class::*)(Args...);

      Delegate(Class& callee, Func func)
         : callee_(callee)
         , func_(func)
         , mutex(new recursive_mutex())
      {}

      ~Delegate()
      {
         delete mutex;
      }

      Return operator()(Args... args) const
      {
         mutex->lock();
         Return result = (callee_.*func_)(args...);
         mutex->unlock();
         return result;
      }

      bool operator==(const Delegate& other) const
      {
         return (&callee_ == &other.callee_) && (func_ == other.func_);
      }
      bool operator!= (const Delegate& other) const
      {
         return !((*this) == other);
      }

   private:
      recursive_mutex   *mutex;
      Class& callee_;
      Func func_;
   };

   /**
   * specialization for const member functions that return void
   */
   template <typename Class, typename... Args>
   class Delegate<void(Class::*)(Args...)>
   {
   public:
      using Func = void(Class::*)(Args...);

      Delegate(Class& callee, Func func)
         : callee_(callee)
         , func_(func)
         , mutex(new recursive_mutex())
      {}

      ~Delegate()
      {
         delete mutex;
      }

      void operator()(Args... args) const
      {
         mutex->lock();
         (callee_.*func_)(args...);
         mutex->unlock();
         return;
      }

      bool operator==(const Delegate& other) const
      {
         return (&callee_ == &other.callee_) && (func_ == other.func_);
      }
      bool operator!= (const Delegate& other) const
      {
         return !(*this == other);
      }

   private:
      recursive_mutex   *mutex;
      Class& callee_;
      Func func_;
   };

   /**
   * specialization for const member functions
   */
   template <typename Class, typename Return, typename... Args>
   class Delegate<Return(Class::*)(Args...) const>
   {
   public:
      using Func = Return(Class::*)(Args...) const;

      Delegate(const Class& callee, Func func)
         : callee_(callee)
         , func_(func)
         , mutex(new recursive_mutex())
      {}

      ~Delegate()
      {
         delete mutex;
      }

      Return operator()(Args... args) const
      {
         mutex->lock();
         Return result = (callee_.*func_)(args...);
         mutex->unlock();
         return result;
      }

      bool operator==(const Delegate& other) const
      {
         return (&callee_ == &other.callee_) && (func_ == other.func_);
      }
      bool operator!= (const Delegate& other) const
      {
         return !(*this == other);
      }

   private:
      recursive_mutex   *mutex;
      const Class& callee_;
      Func func_;
   };

   /**
   * specialization for const member functions that return void
   */
   template <typename Class, typename... Args>
   class Delegate<void(Class::*)(Args...) const>
   {
   public:
      using Func = void(Class::*)(Args...) const;

      Delegate(const Class& callee, Func func)
         : callee_(callee)
         , func_(func)
         , mutex(new recursive_mutex())
      {}

      ~Delegate()
      {
         delete mutex;
      }

      void operator()(Args... args) const
      {
         mutex->lock();
         (callee_.*func_)(args...);
         mutex->unlock();
         return;
      }

      bool operator==(const Delegate& other) const
      {
         return (&callee_ == &other.callee_) && (func_ == other.func_);
      }
      bool operator!= (const Delegate& other) const
      {
         return !(*this == other);
      }

   private:
      recursive_mutex   *mutex;
      const Class& callee_;
      Func func_;
   };

   /**
   * specialization for free functions
   *
   * \tparam R            return type of the function that gets captured
   * \tparam params       variadic template list for possible arguments
   *                      of the captured function
   */
   template <typename Return, typename... Args>
   class Delegate<Return(*)(Args...)>
   {
   public:
      using Func = Return(*)(Args...);

      Delegate(Func func)
         : func_(func)
         , mutex(new recursive_mutex())
      {}

      ~Delegate()
      {
         delete mutex;
      }

      Return operator()(Args... args) const
      {
         mutex->lock();
         Return result = (*func_)(args...);
         mutex->unlock();
         return result;
      }

      bool operator==(const Delegate& other) const
      {
         return func_ == other.func_;
      }
      bool operator!= (const Delegate& other) const
      {
         return !((*this) == other);
      }

   private:
      recursive_mutex   *mutex;
      Func func_;
   };

   /**
   * specialization for functions that return void
   */
   template <typename... Args>
   class Delegate<void(*)(Args...)>
   {
   public:
      using Func = void(*)(Args...);

      Delegate(Func func)
         : func_(func)
         , mutex(new recursive_mutex())
      {}

      ~Delegate()
      {
         delete mutex;
      }

      void operator()(Args... args) const
      {
         mutex->lock();
         (*func_)(args...);
         mutex->unlock();
         return;
      }

      bool operator==(const Delegate& other) const
      {
         return func_ == other.func_;
      }
      bool operator!= (const Delegate& other) const
      {
         return !((*this) == other);
      }

   private:
      recursive_mutex   *mutex;
      Func func_;
   };

   /**
   * function to deduce template parameters from call-context
   */
   template <typename Class, typename Func>
   Delegate<Func> make_delegate(Class& obj, Func func)
   {
      return Delegate<Func>(obj, func);
   }

   template <typename Func>
   Delegate<Func> make_delegate(Func func)
   {
      return Delegate<Func>(func);
   }

   // a little backward compatilbility layer
   #define DELEGATE make_delegate
   #define DELEGATE_CONST make_delegate
   #define DELEGATE_FREE make_delegate
}
#endif // DLGT_DELEGATE_H_