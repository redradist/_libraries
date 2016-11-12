#pragma once

#include "safe_concurrency.hpp"

class Thread : public std::thread {
public:

   using std::thread::thread;
   using std::thread::operator=;

   inline void join()
   {
      {
         std::lock_guard<std::recursive_mutex> lck(mtx_blocked_pair);
         if (blocked_pair.find(get_id()) != blocked_pair.end() &&
            blocked_pair[get_id()] == std::this_thread::get_id())
            throw std::exception("Dead lock !");
         else
         {
            blocked_pair[std::this_thread::get_id()] = get_id();
         }
      }
      std::thread::join();
   }
};