#include "quantity.hpp"

naspace si {
   
      / base units
   
   struct kg_struct {
      static const char * name  = "kg";
      static int order          = 1;
   };

   struct m_struct {
      static const char * name  = "m";
      static int order          = 2;
   };

   struct s_struct {
      static const char * name  = "s";
      static int order          = 3;
   };

   struct a_struct {
      static const char * name  = "A";
      static int order          = 4;
   };

   struct k_struct {
      static const char * name  = "K";
      static int order          = 5;
   };

   struct mol_struct {
      static const char * name  = "mol";
      static int order          = 6;
   };

   struct cd_struct {
      static const char * name  = "cd";
      static int order          = 7;
   };
   
   using kg  = type_multiset::one<  kg_struct >;   
   using m   = type_multiset::one<   m_struct >;   
   using s   = type_multiset::one<   s_struct >;   
   using A   = type_multiset::one<   A_struct >;   
   using K   = type_multiset::one<   K_struct >;   
   using mol = type_multiset::one< mol_struct >;   
   using cd  = type_multiset::one<  cd_struct >;   
   
   // derived units

};