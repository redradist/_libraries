#ifndef PROPERTY_H
#define PROPERTY_H

#include <typeinfo>
#include <stdexcept>

template<typename ClassType, typename ValueType>
struct Property {

   using Getter = ValueType(ClassType::*)(void);
   using Setter = void(ClassType::*)(ValueType);
   
   Property(ClassType *object) noexcept :
      object { object },
      pGet{ nullptr },
      pSet{ nullptr }
   { }

   Property(ClassType *object, Getter getter) noexcept :
      object { object },
      pGet { nullptr },
      pSet { nullptr }
   {
      pGet = getter;
   }

   Property(ClassType *object, Setter setter) noexcept :
      object { object },
      pGet { nullptr },
      pSet { nullptr }
   {
      pSet = setter;
   }

   Property(ClassType *object, Getter getter, Setter setter) noexcept :
      object { object },
      pGet { nullptr },
      pSet { nullptr }
   {
      pGet = getter;
      pSet = setter;
   }

   bool isGetter(void) noexcept
   {
      return pGet ? true : false;
   }

   bool isSetter(void) noexcept
   {
      return pSet ? true : false;
   }

   void operator =(ValueType i) {
      if (pSet != nullptr) (object->*pSet)(i);
      else throw std::invalid_argument("Setter is not provided!");
   }

   operator ValueType() {
      if (pGet != nullptr) return (object->*pGet)();
      else throw std::bad_cast::__construct_from_string_literal("Getter is not provided!");
   }

private:
   ClassType   *object;
   Getter      pGet;
   Setter      pSet;
};

#endif // PROPERTY_H