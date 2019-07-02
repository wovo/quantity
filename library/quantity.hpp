// ==========================================================================
//
// quantity.hpp
//
// a C++ library for expressing and enforcing
// the the distinction between quantities of different things.
//
// https://www.github.com/wovo/quantity
// 
// Copyright Wouter van Ooijen - 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#ifndef quantity_hpp
#define quantity_hpp

#include "type_multiset.hpp"

// this file contains Doxygen lines
/// @file

// Doxygen doeesn't understand concepts or even the
// __attribute__((always_inline)), so those are put
// in @cond INTERNAL / @endcond blocks.


// ==========================================================================
//
// concepts for protecting the quantity operators
//
// I use the phrase 'can be ... with X' to refer to a right operand X.
//
// The concepts are in their own namespace to limit the global name
// space pollution.
//
// ==========================================================================

// doxygen doesn't handle concepts
///@cond INTERNAL

namespace quantity_concepts {
   
// concept for the quantity copy constructor
template< typename V, typename W >
concept bool can_be_constructed_from
= requires ( W w ) {  
   V( w );
};

// concept for the quantity assignment operator
template< typename V, typename W >
concept bool can_be_assigned_from
= requires ( V & v, W w ) {  
   v = w;
};

// concept for the ( + quantity ) operator
template< typename V >
concept bool can_be_plussed
= requires ( V v ) {  
   ( + v );
};

// concept for the ( - quantity ) operator
template< typename V >
concept bool can_be_minussed
= requires ( V v ) {  
   ( - v );
};

// concept for the ( quantity + value ) operator
template< typename V, typename W >
concept bool can_be_added_with_value
= requires ( V v, W w ) {  
   ( v + w );
};

// concept for the ( quantity - value ) operators
template< typename V, typename W >
concept bool can_be_subtracted_with_value 
= requires ( V v, W w ) {  
   ( v - w );
};

// concept for the ( quantity * value ) operator
template< typename V, typename W >
concept bool can_be_multiplied_with_value
= requires ( V v, W w ) {  
   ( v * w );
};

// concept for the ( quantity / value ) operator
template< typename V, typename W >
concept bool can_be_divided_with_value
= requires ( V v, W w ) {  
   ( v / w );
};

// concept for the ( quantity += value ) operator
template< typename V, typename W >
concept bool can_be_update_added_with_value 
= requires ( V v, W w ) {  
   ( v += w );
};

// concept for the ( quantity -= value ) operator
template< typename V, typename W >
concept bool can_be_update_subtracted_with_value 
= requires ( V v, W w ){
   ( v -= w );
};

// concept for the ( quantity == quantity ) operator
template< typename V, typename W >
concept bool can_be_compared_equal 
= requires ( V v, W w ){
   ( v == w );
};

// concept for the ( quantity != quantity ) operator
template< typename V, typename W >
concept bool can_be_compared_unequal 
= requires ( V v, W w ){
   ( v != w );
};

// concept for the ( quantity > quantity ) operator
template< typename V, typename W >
concept bool can_be_compared_larger
= requires ( V v, W w ){
   ( v > w );
};

// concept for the ( quantity >= quantity ) operator
template< typename V, typename W >
concept bool can_be_compared_larger_or_equal
= requires ( V v, W w ){
   ( v >= w );
};

// concept for the ( quantity < quantity ) operator
template< typename V, typename W >
concept bool can_be_compared_smaller
= requires ( V v, W w ){
   ( v < w );
};

// concept for the ( quantity >= quantity ) operator
template< typename V, typename W >
concept bool can_be_compared_smaller_or_equal
= requires ( V v, W w ){
   ( v <= w );
};

// concept for the ( COUT << quantity ) operator
template< typename COUT, typename W >
concept bool can_be_printed_to 
= requires( COUT cout, char c, W w ){
   ( cout << c );
   ( cout << w );
};

// concept for 'unit' equality
template< typename V, typename W >
concept bool compatible = type_multiset::equal< 
   typename V::tags, typename W::tags 
>::value;

}; // namespace quantity_concepts

///@endcond


// ==========================================================================
//
// the quantity template class itself
//
// ==========================================================================

/// ADT for the quantity of some specific thing
//
/// A quantity with a tag type T can be copy-constructed from, 
/// added to, subtracted from, assigned to, and compared with quantities
/// with the same tag type.
///
/// A quantity can be multiplied with or divided by a plain value.
/// Two quantities can be multiplied or divided.
/// For multiplication, the tag type of the result is the sum of the 
/// tag types of the two values, interpreted as type multi-sets.
/// For division, the multiplicities of the second quantity are 
/// subtracted instead of added.
/// When the multi-set of the result type is empty, the resuls a plain
/// value (not a quantity).
///
/// For all operations on quantity values, it is also required that
/// their value types V support the corresponding operation.
/// For operations that produce a result, the value type of the result
/// is the type produced by the corresponding operation on the 
/// value type of the quantities.
///
/// Each quantity type Q has a static constexpr value Q::one,
/// which is used to construct other values of type Q.
///
/// All operations have the __attribute__((always_inline)),
/// hence there is no need to bother with choosing for copy 
/// or reference parameter passing: all passing disappears.
///
template< typename V, typename T >
class quantity_implementation {  
private:

   // For operations that involve a right-hand side quantity,
   // this rhs is a quantity< W, U >.

   // (only) quantities (of any base-type or tag-type) can 
   // - access the value of quantities of any (same or other) type
   // - construct a quantity from a plain (integer) value
   template<typename, typename> friend class quantity_implementation; 
   
   // the stored base type value
   V value;
   
   // the tags (value multiset) of this quantity
   //using tags = T;
   
   // create from a base value
   ///@cond INTERNAL
   __attribute__((always_inline))
   ///@endcond
   constexpr explicit quantity_implementation( const V & value ):
      value( value )
   {}   


public:

   // =======================================================================
   //
   // quantity one constant
   //
   // =======================================================================
   
   /// value one
   //
   /// This value is used to create other quantity values.
   static constexpr const quantity_implementation one = 
      quantity_implementation( 1 );
   

   // =======================================================================
   //
   // constructor and assignment
   //
   // =======================================================================

   /// create in the default way
   ///
   /// Create a quantity with the default value (if any).
   ///@cond INTERNAL
   __attribute__((always_inline))
   ///@endcond
   constexpr explicit quantity_implementation()
   {}

   /// create from another quantity
   ///
   /// Create a quantity from another quantity, which must have 
   /// the same tag type, and a base type
   /// from which our base type can be copy-constructed.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires (
      quantity_concepts::can_be_constructed_from< V, W >
      && quantity_concepts::compatible< T, U >
   )
   __attribute__((always_inline))
   ///@endcond
   constexpr quantity_implementation( 
      const quantity_implementation< W, U > & right 
   ):
      value( right.value )
   {}

   /// assign a quantity from another quantity
   ///
   /// Assign the value from another quantity, 
   /// which must have a the same tag type,
   /// and base type that can be assigned to our base type.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires (
      quantity_concepts::can_be_constructed_from< V, W >
      && quantity_concepts::compatible< T, U >
   )
   __attribute__((always_inline))
   ///@endcond
   quantity_implementation & operator=( 
      const quantity_implementation< W, U > & right 
   ){
      value = right.value;
      return *this;
   }
   

   // =======================================================================
   //
   // add
   //
   // =======================================================================

   /// positive of a quantity
   ///
   /// This unary plus operator just returns the quantity (value) itself.
   constexpr auto operator+() const 
   ///@cond INTERNAL
   requires quantity_concepts::can_be_plussed< V >
   __attribute__((always_inline))
   ///@endcond
   {
      return quantity_implementation< 
         decltype( + value), 
         T 
      >
         ( + value ); 
   }

   /// add a quantity with another quantity
   ///
   /// Add a value to ourself.
   /// The base types of our quantity and the value must be addable.
   /// The result is a quantity of the type 
   /// and with the value of that addition.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_added_with_value< V, W >
   // wovo compatibility!   
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator+( 
      const quantity_implementation< W, U > & right 
   ) const {
      return quantity_implementation< 
         decltype( value + right.value ), 
         T 
      >
         ( value + right.value );      
   }

   /// update add a quantity with a value
   ///
   /// Add a value into ourself.
   /// The base types of our quantity and the value 
   /// must be update addable.
   /// The result is a ourself, updated appropriately.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_update_added_with_value< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   quantity_implementation & operator+=( 
      const quantity_implementation< W, U > & right 
   ){
      value += right.value;
      return *this;
   }


   // =======================================================================
   //
   // subtract
   //
   // =======================================================================

   /// negative of a quantity
   ///
   /// This unary minus operator returns a quantity with the negative
   /// of the value.
   constexpr auto operator-() const 
   ///@cond INTERNAL
   requires quantity_concepts::can_be_minussed< V >
   __attribute__((always_inline))
   ///@endcond
   {
      return quantity_implementation< 
         decltype( - value ), 
         T 
      >
         ( - value ); 
   }

   /// subtract two quantities
   ///
   /// Subtract a quantity from ourself.
   /// The base types of our quantity and the other quantity 
   /// must be subtractable.
   /// The result is of the value type and has the value of that subtraction.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_subtracted_with_value< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator-( 
      const quantity_implementation< W, U > & right 
   ) const {
      return quantity_implementation< 
         decltype( value - right.value ), 
         T 
      >
         ( value - right.value );     
   }
      
   /// update subtract a quantity with a value
   ///
   /// Subtract a value into ourself.
   /// The base types of our quantity and the value 
   /// must be update subtractable.
   /// The result is a ourself, updated appropriately.   
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_update_subtracted_with_value< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   quantity_implementation & operator-=( 
      const quantity_implementation< W, U > & right 
   ){
      value -= right.value;
      return *this;
   }
   
   
   // =======================================================================
   //
   // multiply
   //
   // =======================================================================

   /// multiply a quantity with a plain value
   ///
   /// Multiply ourself with another quantity
   /// The base types of our quantity and the value must be multiplyable.
   /// The result is a quantity of the type 
   /// and with the value of that multiplication.
   template< typename X >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_multiplied_with_value< V, X >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator*( const X & right ) const {
      return quantity_implementation< 
         decltype( value * right ), 
         T  
      >
         ( value * right );      
   }

   /// multiply a quantity with another quantity
   ///
   /// Multiply ourself with another quantity.
   /// The base types of our quantity and the value must be multiplyable.
   /// The result is a quantity of the type 
   /// and with the value of that multiplication.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_multiplied_with_value< V, W >  
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator*( 
      const quantity_implementation< W, U > & right 
   ) const {
      return quantity_implementation< 
         decltype( value * right.value ), 
         type_multiset::add< T, U >
      >
         ( value * right.value );      
   }
   
//       && type_multiset::equal< T, type_multiset::multiply< U, -1 > >::value   

   // =======================================================================
   //
   // divide
   //
   // =======================================================================

   /// divide a quantity with a plain value
   ///
   /// Divide ourself with another quantity
   /// The base types of our quantity and the value must be dividable.
   /// The result is a quantity of the type 
   /// and with the value of that divison.
   template< typename X >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_divided_with_value< V, X >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator/( const X & right ) const {
      return quantity_implementation< 
         decltype( value / right ), 
         T  
      >
         ( value / right );      
   }

   /// divide a quantity with another quantity
   ///
   /// Divide ourself with another quantity.
   /// The base types of our quantity and the value must be dividable.
   /// The result is a quantity of the type 
   /// and with the value of that division.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_divided_with_value< V, W >  
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator/( 
      const quantity_implementation< W, U > & right 
   ) const {
      return quantity_implementation< 
         decltype( value / right.value ), 
         type_multiset::add< T, type_multiset::multiply< U, -1 > >
      >
         ( value / right.value );      
   }

   /// divide a quantity with another quantity
   ///
   /// Divide ourself with another quantity of opposite units.
   /// The base types of our quantity and the value must be dividable.
   /// The result is a quantity of the type 
   /// and with the value of that division.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires (
      quantity_concepts::can_be_divided_with_value< V, W >  
      && type_multiset::equal< T, U >::value
   )      
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator/( 
      const quantity_implementation< W, U > & right 
   ) const {
      return value / right.value;
   }
   

   // =======================================================================
   //
   // compare equal
   //
   // =======================================================================

   /// compare quantities for equality
   ///
   /// Compare two quantities for equality.
   /// The base types of te quantities must support comparing for equality.
   /// The result is te result of that comparison.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_equal< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator==( 
      const quantity_implementation< W, U > & right 
   ) const {
      return value == right.value;
   }

   /// compare quantities for inequality
   ///
   /// Compare two quantities for inequality.
   /// The base types of te quantities must support comparing for inequality.
   /// The result is te result of that comparison.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_unequal< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator!=( 
      const quantity_implementation< W, U > & right 
   ) const {
      return value != right.value;
   }
   

   // =======================================================================
   //
   // compare larger
   //
   // =======================================================================

   /// compare quantities for larger
   ///
   /// Compares a quantity for being larger than another quantity.
   /// The base types of te quantities must support the comparison.
   /// The result is te result of that comparison.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_larger< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator>( 
      const quantity_implementation< W, U > & right 
   ) const {
      return value > right.value;
   }

   /// compare quantities for larger or equal
   ///
   /// Compares a quantity for being larger than or equal to another quantity.
   /// The base types of te quantities must support the comparison.
   /// The result is te result of that comparison.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_larger_or_equal< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator>=( 
      const quantity_implementation< W, U > & right 
   ) const {
      return value >= right.value;
   }


   // =======================================================================
   //
   // compare smaller
   //
   // =======================================================================

   /// compare quantities for smaller
   ///
   /// Compares a quantity for being smaller than another quantity.
   /// The base types of te quantities must support the comparison.
   /// The result is te result of that comparison.
   template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_smaller< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator<( 
      const quantity_implementation< W, U > & right 
   ) const {
      return value < right.value;
   }

   /// compare quantities for smaller or equal
   ///
   /// Compares a quantity for being smaller than or equal to another quantity.
   /// The base types of te quantities must support the comparison.
   /// The result is te result of that comparison.
      template< typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_smaller_or_equal< V, W >
   // wovo compatibility!
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator<=( 
      const quantity_implementation< W, U > & right 
   ) const {
      return value <= right.value;
   }
     
   // =======================================================================
   //
   // reverse arithmetic
   //
   // * and / with plain values
   //
   // =======================================================================

/*
   // stopgap because friend doesn't seem to work properly for operator+
   ///@cond INTERNAL
   __attribute__((always_inline))
   constexpr explicit quantity( const T & value, int n ):
      value( value )
   {}   
   ///@endcond
   
   /// add a value and a quantity
   ///
   /// Add ourself to a value.
   /// The base types of our quantity and the value must be addable.
   /// The result is a quantity of the type 
   /// and with the value of that addition.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_added_with_value< U, T >
   __attribute__((always_inline))
   ///@endcond
   friend constexpr auto operator+( const U & left, const quantity & right ){ 
      return quantity< decltype( left + right.value ), M >( 
        left + right.value, 42 ); 
   }
*/

}; // template class quantity

// the user interface: wrap the user's T as a singleton type_multiset
template< typename V, typename T >
struct quantity final : 
   quantity_implementation< V, type_multiset::one< T >> {};


// ==========================================================================
//
// print
//
// ==========================================================================

/// print a quantity to a cout-like object
///
/// The quantity value is printed, followed by the 
/// tags list.
/// The left argument must support printing (using operator<<)
/// of a base type value.
template< typename COUT, typename V, typename T >
///@cond INTERNAL
requires quantity_concepts::can_be_printed_to< COUT, V >
///@endcond
COUT & operator<<( 
   COUT & cout, 
   const quantity_implementation< V, T > & right 
){
   cout << right / quantity_implementation< V, T >::one;
   type_multiset::print< T >( cout );
   return cout;
}
   
#endif // ifndef quantity_hpp