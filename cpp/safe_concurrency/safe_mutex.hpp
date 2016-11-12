#pragma once

#include "safe_concurrency.hpp"

class RecursiveMutex : std::recursive_mutex {
public:

   using std::recursive_mutex::recursive_mutex;
   using std::recursive_mutex::operator=;

   inline void lock()
   {
      if (try_lock())
      {
         _id = std::this_thread::get_id();
      }
      else
      {
         {
            std::lock_guard<std::recursive_mutex> lck(mtx_blocked_pair);
            std::thread::id id = _id;
            if (blocked_pair.find(id) != blocked_pair.end() &&
               blocked_pair[id] == std::this_thread::get_id())
               throw std::exception("Dead lock !");
            else
            {
               blocked_pair[std::this_thread::get_id()] = id;
            }
         }
         std::recursive_mutex::lock();
      }
   }

private:
   std::thread::id _id;
};