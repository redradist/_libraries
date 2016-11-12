#pragma once
#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <vector>
#include <mutex>
#include <memory>

template<typename T>
class callback;

template<typename Result, typename ... Args>
class callback<Result(Args...)>
{
public:

   callback()
   : mtx{ std::recursive_mutex() }
   , delegate{ nullptr }
   {

   }

   template<typename Type>
   callback(callback && obj)
   : mtx{ std::move(obj.mtx) }
   , delegate{ obj.delegate }
   {

   }

   template<typename Type>
   callback(Type func_obj)
   : delegate{ func_obj }
   {

   }

   template<typename Type>
   void operator =(Type func_obj)
   {
      delegate = func_obj;
   }

   Result operator ()(Args... args)
   {
      std::lock_guard<std::recursive_mutex> lck(mtx);
      return delegate(args...);
   }

private:
   std::recursive_mutex mtx;
   std::function<Result(Args...)> delegate;
};

template<typename ... Args>
class callback<void(Args...)>
{
public:

   callback(callback & obj) = delete;

   callback() = default;
   callback(callback && obj) = default;

   template<typename Type>
   callback(Type func_obj)
   {
      delegate = func_obj;
   }

   template<typename Class>
   callback(void(Class::*func)(Args...), Class *obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func, obj, _1);
   }

   template<typename Class>
   callback(void(Class::*func)(Args...), Class &obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func, &obj, _1);
   }

   template<typename Class>
   callback(void(Class::*func)(Args...) const, Class const *obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func, obj, _1);
   }

   template<typename Class>
   callback(void(Class::*func)(Args...) const, Class const &obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func, &obj, _1);
   }

   template<typename Class>
   callback(std::pair<void(Class::*)(Args...), Class *> func_obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, func_obj.second, _1);
   }

   template<typename Class>
   callback(std::pair<void(Class::*)(Args...), Class &> func_obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, &func_obj.second, _1);
   }

   template<typename Class>
   callback(std::pair<void(Class::*)(Args...) const, Class *> func_obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, func_obj.second, _1);
   }

   template<typename Class>
   callback(std::pair<void(Class::*)(Args...) const, Class &> func_obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, &func_obj.second, _1);
   }

   template<typename Class>
   callback(std::pair<void(Class::*)(Args...) const, Class const*> func_obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, func_obj.second, _1);
   }

   template<typename Class>
   callback(std::pair<void(Class::*)(Args...) const, Class const &> func_obj)
   {
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, &func_obj.second, _1);
   }

   void operator =(callback && dlg)
   {
      std::lock_guard<std::recursive_mutex> lck(mtx);
      delegate = dlg.delegate;
   }

   template<typename Type>
   void operator =(Type func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(mtx);
      delegate = func_obj;
   }

   template<typename Class>
   void operator =(std::pair<void(Class::*)(Args...), Class*> & func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(mtx);
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, func_obj.second, _1);
   }

   template<typename Class>
   void operator =(std::pair<void(Class::*)(Args...) const, Class const*> & func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(mtx);
      using std::placeholders::_1;
      delegate = std::bind(func_obj.first, func_obj.second, _1);
   }

   void operator ()(Args... args)
   {
      std::lock_guard<std::recursive_mutex> lck(mtx);
      delegate(args...);
   }

private:
   std::recursive_mutex mtx;
   std::function<void(Args...)> delegate;
};

#endif // CALLBACK_H