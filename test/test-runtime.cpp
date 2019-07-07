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
// multiset tests 
//
// ==========================================================================

struct tag_a { static const char name = 'a'; };
struct tag_b { static const char name = 'b'; };
struct tag_c { static const char name = 'c'; };
struct tag_d { static const char name = 'd'; };

using a = type_multiset::one< tag_a >;
using b = type_multiset::one< tag_b >;
using c = type_multiset::one< tag_c >;
using d = type_multiset::one< tag_d >;

using a2    = type_multiset::add< a, a >;
using b2    = type_multiset::add< b, b >;
using c2    = type_multiset::add< c, c >;
using ab    = type_multiset::add< b, a >;
using ba    = type_multiset::add< a, b >;
using a2b2  = type_multiset::add< b2, a2 >;
using b2a2  = type_multiset::add< a2, b2 >;
using a2b   = type_multiset::add< b, a2 >;
using b2a   = type_multiset::add< a, b2 >;
using ab2   = type_multiset::add< b2, a >;
using ba2   = type_multiset::add< a2, b >;

void test_multiset_empty(){
   std::stringstream s;
   
   s.str( "" );
   type_multiset::print< type_multiset::empty >( s );
   CHECK_EQUAL( s.str(), "" );
}

void test_multiset_one(){
   std::stringstream s;
   
   s.str( "" );
   type_multiset::print< type_multiset::one< tag_a >>( s );
   CHECK_EQUAL( s.str(), "a" );
   
   s.str( "" );
   type_multiset::print< type_multiset::one< tag_b >>( s );
   CHECK_EQUAL( s.str(), "b" );
}

void test_multiset_add_element(){
   std::stringstream s;
   
   s.str( "" );
   type_multiset::print< 
      type_multiset::add_element< tag_a, 1, type_multiset::empty >
   >( s );
   CHECK_EQUAL( s.str(), "a" )	
	
   s.str( "" );
   type_multiset::print< 
      type_multiset::add_element< tag_a, 1, a >
   >( s );
   CHECK_EQUAL( s.str(), "a2" )	
	
   s.str( "" );
   type_multiset::print< 
      type_multiset::add_element< tag_a, 1, b >
   >( s );
   CHECK_EQUAL( s.str(), "ba" )	
	
   s.str( "" );
   type_multiset::print< 
      type_multiset::add_element< tag_a, 1, ab >
   >( s );
   CHECK_EQUAL( s.str(), "a2b" )	
   
   s.str( "" );
   type_multiset::print< 
      type_multiset::add_element< tag_a, 1, ba >
   >( s );
   CHECK_EQUAL( s.str(), "ba2" )	
	
   s.str( "" );
   type_multiset::print< 
      type_multiset::add_element< tag_c, 1, b2a2 >
   >( s );
   CHECK_EQUAL( s.str(), "b2a2c" )	
   
}

void test_multiset_add(){
   std::stringstream s;
   
   s.str( "" );
   type_multiset::print< a2 >( s );
   CHECK_EQUAL( s.str(), "a2" )	
	
   s.str( "" );
   type_multiset::print< b2 >( s );
   CHECK_EQUAL( s.str(), "b2" )	
	
   s.str( "" );
   type_multiset::print< ab >( s );
   CHECK_EQUAL( s.str(), "ab" )	
	
   s.str( "" );
   type_multiset::print< ba >( s );
   CHECK_EQUAL( s.str(), "ba" )	
	
   s.str( "" );
   type_multiset::print< a2b2 >( s );
   CHECK_EQUAL( s.str(), "a2b2" )	

   s.str( "" );
   type_multiset::print< b2a2 >( s );
   CHECK_EQUAL( s.str(), "b2a2" )	

   s.str( "" );
   type_multiset::print< a2b >( s );
   CHECK_EQUAL( s.str(), "a2b" )	

   s.str( "" );
   type_multiset::print< b2a >( s );
   CHECK_EQUAL( s.str(), "b2a" )	

   s.str( "" );
   type_multiset::print< ab2 >( s );
   CHECK_EQUAL( s.str(), "ab2" )	

   s.str( "" );
   type_multiset::print< ba2 >( s );
   CHECK_EQUAL( s.str(), "ba2" )	

   // add to front
   s.str( "" );
   type_multiset::print< type_multiset::add< a, a2b2 >>( s );
   CHECK_EQUAL( s.str(), "a3b2" )	
	
   // add to tail
   s.str( "" );
   type_multiset::print< type_multiset::add< b, a2b2 >>( s );
   CHECK_EQUAL( s.str(), "a2b3" )	
	
   // add new before tail
   s.str( "" );
   type_multiset::print< type_multiset::add< c, a2b2 >>( s );
   CHECK_EQUAL( s.str(), "a2b2c" )	
	
   // add none to none
   s.str( "" );
   type_multiset::print< type_multiset::add< type_multiset::empty, type_multiset::empty >>( s );
   CHECK_EQUAL( s.str(), "" )	
	
   // add none to non-empty
   s.str( "" );
   type_multiset::print< type_multiset::add< type_multiset::empty, a2b2 >>( s );
   CHECK_EQUAL( s.str(), "a2b2" )	
	
   // add list to empty
   s.str( "" );
   type_multiset::print< type_multiset::add< a2b2, type_multiset::empty >>( s );
   CHECK_EQUAL( s.str(), "b2a2" )	
	
   // add list to list
   s.str( "" );
   type_multiset::print< type_multiset::add< a2b2, a2 >>( s );
   CHECK_EQUAL( s.str(), "a4b2" )	
	
}

void test_multiset_multiply(){
   std::stringstream s;
   
   // multiply empty
   s.str( "" );
   type_multiset::print< type_multiset::multiply< type_multiset::empty, 12 >>( s );
   CHECK_EQUAL( s.str(), "" )	
	
   // multiply single
   s.str( "" );
   type_multiset::print< type_multiset::multiply< a, 15 >>( s );
   CHECK_EQUAL( s.str(), "a15" )	
	
   // multiply list
   s.str( "" );
   type_multiset::print< type_multiset::multiply< a2b2, 15 >>( s );
   CHECK_EQUAL( s.str(), "a30b30" )	
   
   // multiply prune single
   s.str( "" );
   type_multiset::print< type_multiset::multiply< a, 0 >>( s );
   CHECK_EQUAL( s.str(), "" )	
	
   // multiply prune list
   s.str( "" );
   type_multiset::print< type_multiset::multiply< a2b2, 0 >>( s );
   CHECK_EQUAL( s.str(), "" )	
	
}

void test_multiset_add_prune(){
   std::stringstream s;
   
   // result is empty
   s.str( "" );
   type_multiset::print< type_multiset::add <
      ab,
      type_multiset::multiply< ab, -1 >
   >>( s );
   CHECK_EQUAL( s.str(), "" )	
	   
   // subtract, chain remains the same
   s.str( "" );
   type_multiset::print< 
      type_multiset::add < type_multiset::multiply< a, -1 >, a2b2 >
   >( s );
   CHECK_EQUAL( s.str(), "ab2" )	
   
   // result is half-empty
   s.str( "" );
   type_multiset::print< 
      type_multiset::add < type_multiset::multiply< a, -2 >, a2b2 >
   >( s );
   CHECK_EQUAL( s.str(), "b2" )	
	
}

void test_multiset_equal(){

   { auto x = type_multiset::equal< 
      type_multiset::empty, 
      type_multiset::empty 
   >::value;  
   CHECK_EQUAL( x, true ); }
	
   { auto x = type_multiset::equal< 
      type_multiset::empty, 
      a 
   >::value;  
   CHECK_EQUAL( x, false ); }
	
   { auto x = type_multiset::equal< 
      a,
      type_multiset::empty
   >::value;  
   CHECK_EQUAL( x, false ); }

   { auto x = type_multiset::equal< 
      a,
      a
   >::value;  
   CHECK_EQUAL( x, true ); }

   { auto x = type_multiset::equal< 
      ab,
      ab
   >::value;  
   CHECK_EQUAL( x, true ); }

   { auto x = type_multiset::equal< 
      ba,
      ab
   >::value;  
   CHECK_EQUAL( x, true ); }

   { auto x = type_multiset::equal< 
      a2b2,
      a2b2
   >::value;  
   CHECK_EQUAL( x, true ); }

   { auto x = type_multiset::equal< 
      b2a2,
      a2b2
   >::value;  
   CHECK_EQUAL( x, true ); }

   { auto x = type_multiset::equal< 
      a,
      ab
   >::value;  
   CHECK_EQUAL( x, false ); }

   { auto x = type_multiset::equal< 
      b,
      ab
   >::value;  
   CHECK_EQUAL( x, false ); }

   { auto x = type_multiset::equal< 
      a2b,
      a2b2
   >::value;  
   CHECK_EQUAL( x, false ); }

   { auto x = type_multiset::equal< 
      ab2,
      a2b2
   >::value;  
   CHECK_EQUAL( x, false ); }

}

// ==========================================================================
//
// quantity tests 
//
// ==========================================================================

using qa = quantity< int, tag_a >;
using qb = quantity< int, tag_b >;

void test_constructor(){
   std::stringstream s;
   
   // value one
   s.str( "" );
   s << qa::one;
   CHECK_EQUAL( s.str(), "1a" )
   
   s.str( "" );
   s << qb::one;
   CHECK_EQUAL( s.str(), "1b" )

}

void test_divide(){
   std::stringstream s;
 
   // divide by a scalar
   s.str( "" );
   s << qa::one / 1;
   CHECK_EQUAL( s.str(), "1a" )
   
   // divide by another quantity
   s.str( "" );
   s << qa::one / qb::one;
   CHECK_EQUAL( s.str(), "1b-1a" )   
   
   // divide by the same quantity
   s.str( "" );
   s << ( qb::one * 6 ) / ( qb::one * 3 );
   CHECK_EQUAL( s.str(), "2" )   
   
   // reverse divide
   s.str( "" );
   s << 3 / qb::one;
   CHECK_EQUAL( s.str(), "3b-1" )   
}

void test_multiply(){
   std::stringstream s;
 
   // multiply by a scalar
   s.str( "" );
   s << qa::one * 14;
   CHECK_EQUAL( s.str(), "14a" )
   
   // multiply by another quantity
   s.str( "" );
   s << ( qb::one * 5 ) * ( qa::one * 3 );
   CHECK_EQUAL( s.str(), "15ab" )   

   // multiply by the inverse qunatity
   s.str( "" );
   s << ( qb::one * 6 ) * ( 2 / qb::one );
   CHECK_EQUAL( s.str(), "12" )   
   
   // reverse multiply
   s.str( "" );
   s << 15 * qa::one;
   CHECK_EQUAL( s.str(), "15a" )   
}




// ==========================================================================
//
// main
//
// ==========================================================================

int main(){
   test_multiset_empty();
   test_multiset_one();
   test_multiset_add_element();
   test_multiset_add();
   test_multiset_multiply();
   test_multiset_add_prune();
   test_multiset_equal();
	
   test_constructor();
   test_divide();
   test_multiply();


   return test_end();
}