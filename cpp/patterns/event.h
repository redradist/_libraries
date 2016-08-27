#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <vector>
#include <mutex>
#include <memory>

#define EVENT(Name, Owner, ...) \
struct Name ## Owner : event<void(__VA_ARGS__)> { friend class Owner; }

template<typename T>
class event;

template<typename Result, typename ... Args>
class event<Result(Args...)>;

template<typename ... Args>
class event<void(Args...)>
{
public:

   event & evt;

   event() 
   : evt{ *this }
   { }

   event(event const & evt)
   : evt{ *this }
   {
      if (this != &evt)
      {
         mtx = evt.mtx;
         functions = evt.functions;
      }
   }

   template<typename Class>
   event(void(Class::*func)(Args...), Class* obj)
      : evt{ *this }
   {
      *this += std::make_pair(func, obj);
   }

   template<typename Class>
   event(void(Class::*func)(Args...) const, Class const* obj)
   : evt{ *this }
   {
      *this += std::make_pair(func, obj);
   }

   template<typename Class>
   event(void(Class::*func)(Args...), Class& obj)
      : evt{ *this }
   {
      *this += std::make_pair(func, &obj);
   }

   template<typename Class>
   event(void(Class::*func)(Args...) const, Class const& obj)
      : evt{ *this }
   {
      *this += std::make_pair(func, &obj);
   }
   
   void operator +=(event && evt)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      functions = evt.functions;
   }

   template<typename Type>
   void operator +=(Type func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      functions.push_back(func_obj);
   }

   template<typename Class>
   void operator +=(std::pair<void(Class::*)(Args...), Class*> && func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      using std::placeholders::_1;
      functions.push_back(std::bind(func_obj.first, func_obj.second, _1));
   }

   template<typename Class>
   void operator +=(std::pair<void(Class::*)(Args...) const, Class const*> && func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      using std::placeholders::_1;
      functions.push_back(std::bind(func_obj.first, func_obj.second, _1));
   }

   virtual ~event()
   {
      
   }

protected:

   virtual void operator ()(Args... args)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      for (auto & function : functions)
      {
         function(args...);
      }
   }

private:

   std::shared_ptr<std::recursive_mutex> mtx =
      std::make_shared<std::recursive_mutex>();
   std::vector<std::function<void(Args...)>> functions;
};

#endif // EVENT_H