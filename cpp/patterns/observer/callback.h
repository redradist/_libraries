#ifndef CALLBACK_H
#define CALLBACK_H

#include "observer.h"
#include "mutex"

namespace callback
{
   using namespace std;
   using namespace observer;

   template <typename Return, typename... Args>
   class CallBack : public Observer<Return, Args...>
   {
   private:
      using Func = Return(*)(Args...);
      Func   mFunc; // Address of the function on the delegate object.

   public:
      CallBack(Func aFunc)
         : mFunc(aFunc) { }

      virtual Return operator()(Args... args) const
      {
         return (*mFunc)(args...);
      }
   }; // Static
}

#endif // CALLBACK_H