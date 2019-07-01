// ==========================================================================
//
// test-runtime.hpp
//
// torsor runtime tests
//
// https://www.github.com/wovo/torsor
// 
// Copyright Wouter van Ooijen - 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include <string>
#include <sstream>
#include <iostream>
#include "quantity.hpp"


// ==========================================================================
//
// simple test framework
//
// ==========================================================================

int tests_total = 0;
int tests_failed = 0;

template< typename A, typename B >
void test_ok( 
   bool ok,                          // Test OK?
   const char *name,                 // name of the test
   const char * f, int n,            // file name and line number
   const char * ta, const char *tb,  // arguments, stringified
   const A & a, const B & b          // arguments, as-is
){
   ++tests_total;	
   if( ! ok ){
      ++tests_failed;
      std::cout 
         << f << ":" << std::dec << n 
         << " check failed \n" 
         << "   " << name << "( " 
         << ta;
      if( tb != 0){         
         std::cout << " , " << tb;
      }         
      std::cout << " )\n"
         << "   left  \"" << ta << "\" = " << a << "\n";
      if( tb != 0 ){   
         std::cout << "   right \"" << tb << "\" = " << b << "\n";
      }         
      std::cout << "\n";
   }
}

#define CHECK_EQUAL( a, b ) \
   test_ok( (a) == (b), "CHECK_EQUAL",  __FILE__, __LINE__, #a, #b, a, b );   

#define CHECK_NOT_EQUAL( a, b ) \
   test_ok( (a) != (b), "CHECK_NOT_EQUAL", __FILE__, __LINE__, #a, #b, a, b );   

#define CHECK_TRUE( a ) \
   test_ok( (a), "CHECK_TRUE", __FILE__, __LINE__, #a, 0, a, 0 );   

#define CHECK_FALSE( a ) \
   test_ok( !(a), "CHECK_TRUE", __FILE__, __LINE__, #a, 0, a, 0 );   

int test_end(){
   if( tests_failed == 0 ){
      std::cout 
	     << "\nRuntime test success: " 
		 << std::dec << tests_total
		 << " test(s) were successfull\n";
      return 0;
   } else {	   
      std::cout 
	     << "\nRUNTIME TEST FAILURE: " 
		 << std::dec << tests_failed
		 << " test(s) were NOT successfull\n";
      return -1;
   }
}


// ==========================================================================
//
// the tests 
//
// ==========================================================================

struct tag_a { };
struct tag_b { };

using qa = quantity< int, tag_a >;
using qb = quantity< int, tag_b >;

void test_constructor(){
   auto a = qa::one * 2;
   auto b = qb::one * 40;
   auto x = a + b;
   std::cout << x;
}



int main(){
   test_constructor();


   return test_end();
}