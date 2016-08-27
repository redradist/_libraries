#ifndef PROPERTY_H
#define PROPERTY_H

#include <typeinfo>
#include <functional>
#include <stdexcept>

template<typename ValueType>
struct property {

   using Getter = std::function<ValueType(void)>;
   using Setter = std::function<void(ValueType)>;
   
   property() noexcept = default;

   property(Getter getter) noexcept
   : pGet{ getter }
   {

   }

   property(Setter setter) noexcept
   : pSet{ setter }
   {

   }

   property(Getter getter, Setter setter) noexcept
   : pGet{ getter }
   , pSet{ setter }
   {

   }

   bool isGetter(void) noexcept
   {
      return pGet ? true : false;
   }

   bool isSetter(void) noexcept
   {
      return pSet ? true : false;
   }

   void operator =(ValueType i) 
   {
      if (pSet != nullptr) pSet(i);
      else throw std::invalid_argument("Setter is not provided!");
   }

   operator ValueType() const
   {
      if (pGet != nullptr) return pGet();
      else throw std::bad_cast::__construct_from_string_literal("Getter is not provided!");
   }

   friend std::ostream& operator <<(std::ostream& os, const property& obj)
   {
      return os << static_cast<ValueType>(obj);
   }

   friend std::istream& operator >>(std::istream& is, property& obj)
   {
      ValueType value;
      is >> value;
      obj = value;
      return is;
   }

private:
   Getter      pGet = nullptr;
   Setter      pSet = nullptr;
};

#endif // PROPERTY_H