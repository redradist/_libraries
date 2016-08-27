#pragma once

#include <iostream>
#include <typeinfo>
#include <exception>
#include <stdexcept>

namespace flthrow {
   template<typename T>
   inline void flthrow(T type, std::string error_occure);

   template<typename T : public std::exception>
   inline void flthrow(T type, std::string error_occure)
   {
      class Type : std::exception {
         std::string error_occure;
      public:
         Type(T type, std::string error_occure) :
            std::exception((std::string(typeid(type).name()) + " is " + std::to_string(type)).c_str()),
            error_occure(error_occure + " : => " + std::exception::what()) { }

         virtual char const* what() const
         {
            return error_occure.c_str();
         }
      };
      throw Type(type, error_occure);
   }

   template<typename T>
   inline void flthrow(T *type, std::string error_occure)
   {
      class PointType : public std::exception {
         T *&type;
         std::string error_occure;
      public:
         PointType(T *type, std::string error_occure) :
            type(type),
            std::exception(std::string(typeid(type).name()).c_str()),
            error_occure(error_occure + " : => " + std::exception::what()) { }
         
         PointType(PointType const & pType) : 
            type(pType.type) { }

         ~PointType()
         {
            if (type != NULL)
            {
               delete type;
            }
            type = NULL;
         }

         virtual char const* what() const
         {
            return error_occure.c_str();
         }
      };
      throw PointType(type, error_occure);
   }

   template<>
   inline void flthrow(std::exception type, std::string error_occure)
   {
      class Type : public std::exception {
         std::string error_occure;

      public:
         Type(std::exception type, std::string error_occure) :
            std::exception(type),
            error_occure(error_occure + " : => " + std::exception::what()) { }

         virtual char const* what() const
         {
            return error_occure.c_str();
         }
      };
      throw Type(type, error_occure);
   }

   template<>
   inline void flthrow(std::logic_error &type, std::string error_occure)
   {
      class Type : public std::logic_error {
         std::string error_occure;
      public:
         Type(std::logic_error type, std::string error_occure) :
            std::logic_error(type),
            error_occure(error_occure + " : => " + std::logic_error::what()) { }
         
         virtual char const* what() const
         {
            return error_occure.c_str();
         }
      };
      throw Type(type, error_occure);
   }

   template<>
   inline void flthrow(std::system_error &type, std::string error_occure)
   {
      class Type : public std::system_error {
         std::string error_occure;
      public:
         Type(std::system_error type, std::string error_occure) :
            std::system_error(type),
            error_occure(error_occure + " : => " + std::system_error::what()) { }
         
         virtual char const* what() const
         {
            return error_occure.c_str();
         }
      };
      throw Type(type, error_occure);
   }
};

#define fthrow(type) flthrow::flthrow(type, std::string(std::string(__FILE__) + " : line " + std::to_string(__LINE__)))
