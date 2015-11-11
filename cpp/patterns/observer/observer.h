#ifndef OBSERVER_H
#define OBSERVER_H

namespace observer
{
   template <typename Return, typename... Args>
   class Observer
   {
   public:
      virtual ~Observer() { }
      virtual inline Return operator()(Args... args) const = 0;
   };
}

#endif // OBSERVER_H