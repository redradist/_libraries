#pragma once
#ifndef DELEGATE_H
#define DELEGATE_H

#include <functional>
#include <vector>
#include <mutex>
#include <memory>

#define DELEGATE(Name, Owner, Result, ...) \
struct Name ## Owner : delegate<Result(__VA_ARGS__)> { friend class Owner; }

template<typename T>
class delegate;

template<typename Result, typename ... Args>
class delegate<Result(Args...)>
{
public:

   delegate & dlg;

   delegate()
   : dlg{ *this }
   { }

   delegate(delegate const & dlg)
   : dlg{ *this }
   {
      if (this != &dlg)
      {
         mtx = dlg.mtx;
         functions = dlg.functions;
      }
   }

   template<typename Type>
   void operator +=(Type func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      functions.push_back(func_obj);
   }

   template<typename Class>
   void operator +=(std::pair<Result(Class::*)(Args...), Class*> func_obj)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      using std::placeholders::_1;
      functions.push_back(std::bind(func_obj.first, func_obj.second, _1));
   }

   virtual ~delegate()
   {
      
   }

protected:

   virtual std::vector<Result> operator ()(Args... args)
   {
      std::lock_guard<std::recursive_mutex> lck(*mtx);
      std::vector<Result> results;
      for (auto & function : functions)
      {
         results.push_back(function(args...));
      }
      return results;
   }

private:
   std::shared_ptr<std::recursive_mutex> mtx = 
      std::make_shared<std::recursive_mutex>();
   std::vector<std::function<Result(Args...)>> functions;
};

#endif // DELEGATE_H