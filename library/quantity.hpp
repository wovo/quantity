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

}; // namespace quantity_concepts

///@endcond


// ==========================================================================
//
// the quantity template class itself
//
// ==========================================================================

/// ADT for the quantity of some specific thing
//
/// A quantity type with a specific tag can be added to, 
/// subtracted from, assigned to, and compared with quantities
/// with the same tag.
/// A quantity can be multiplied with or divided by a plain value.
///
/// All operations have the __attribute__((always_inline)),
/// hence there is no need to bother with choosing for copy 
/// or reference parameter passing: all passing disappears.
///
template< typename T, typename M = void >
class quantity final {  
private:

   // (only) quantities (of any type) can 
   // - access the value of quantities of any (same or other) type
   template<typename, typename> friend class quantity; 
   
   // the stored base type value
   T value;


public:

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
   constexpr explicit quantity()
   {}

   /// create from a base value
   ///
   /// Create a quantity with some value.
   ///@cond INTERNAL
   __attribute__((always_inline))
   ///@endcond
   constexpr explicit quantity( const T & value ):
      value( value )
   {}

   /// create from another quantity
   ///
   /// Create a quantity from another quantity, which must have a base type
   /// from which our own base type can be copy-constructed.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_constructed_from< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr quantity( const quantity< U, M > & right ):
      value( right.value )
   {}

   /// assign a quantity from another quantity
   ///
   /// Copy the value from another quantity, which must have a base type
   /// that can be assigned to our base type.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_assigned_from< T, U >
   __attribute__((always_inline))
   ///@endcond
   quantity & operator=( const quantity< U, M > & right ){
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
   /// This unary plus operator just returns the quantity (value) itself.w
   constexpr auto operator+() const 
   ///@cond INTERNAL
   requires quantity_concepts::can_be_plussed< T >
   __attribute__((always_inline))
   ///@endcond
   {
      return ::quantity< T, M >( + value ); 
   }

   /// add a quantity with a value
   ///
   /// Add a value to ourself.
   /// The base types of our quantity and the value must be addable.
   /// The result is a quantity of the type 
   /// and with the value of that addition.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_added_with_value< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator+( const U & right ) const {
      return ::quantity< decltype( value + right ), M >( value + right );      
   }

   /// update add a quantity with a value
   ///
   /// Add a value into ourself.
   /// The base types of our quantity and the value 
   /// must be update addable.
   /// The result is a ourself, updated appropriately.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_update_added_with_value< T, U >
   __attribute__((always_inline))
   ///@endcond
   quantity & operator+=( const U & right ){
      value += right;
      return *this;
   }


   // =======================================================================
   //
   // subtract
   //
   // =======================================================================

   /// subtract a quantity with a value
   ///
   /// Subtract a value from ourself.
   /// The base types of our quantity and the value must be subtractable.
   /// The result is a quantity of the type 
   /// and with the value of that subtraction.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_subtracted_with_value< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator-( const U & right ) const {
      return ::quantity< decltype( value - right ), M >( value - right );     
   }
   
   /// subtract two quantities
   ///
   /// Subtract a quantity from ourself.
   /// The base types of our quantity and the other quantity 
   /// must be subtractable.
   /// The result is of the type and has the value of that subtraction.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_subtracted_with_value< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator-( const quantity< U, M > & right ) const {
      return value - right.value;      
   }
   
   /// update subtract a quantity with a value
   ///
   /// Subtract a value into ourself.
   /// The base types of our quantity and the value 
   /// must be update subtractable.
   /// The result is a ourself, updated appropriately.   
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_update_subtracted_with_value< T, U >
   __attribute__((always_inline))
   ///@endcond
   quantity & operator-=( const U & right ){
      value -= right;
      return *this;
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
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_equal< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator==( const quantity< U, M > & right ) const {
      return value == right.value;
   }

   /// compare quantities for inequality
   ///
   /// Compare two quantities for inequality.
   /// The base types of te quantities must support comparing for inequality.
   /// The result is te result of that comparison.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_unequal< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator!=( const quantity< U, M > & right ) const {
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
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_larger< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator>( const quantity< U, M > & right ) const {
      return value > right.value;
   }

   /// compare quantities for larger or equal
   ///
   /// Compares a quantity for being larger than or equal to another quantity.
   /// The base types of te quantities must support the comparison.
   /// The result is te result of that comparison.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_larger_or_equal< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator>=( const quantity< U, M > & right ) const {
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
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_smaller< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator<( const quantity< U, M > & right ) const {
      return value < right.value;
   }

   /// compare quantities for smaller or equal
   ///
   /// Compares a quantity for being smaller than or equal to another quantity.
   /// The base types of te quantities must support the comparison.
   /// The result is te result of that comparison.
   template< typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_compared_smaller_or_equal< T, U >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator<=( const quantity< U, M > & right ) const {
      return value <= right.value;
   }
     
   // =======================================================================
   //
   // reverse arithmetic
   //
   // =======================================================================

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


}; // template class quantity


// ==========================================================================
//
// print
//
// ==========================================================================

/// print a quantity to a cout-like object
///
/// The quantity value is printed, preceded by a '@'
/// character.
/// The left argument must support printing (using operator<<)
/// of a char and of a base type value.
template< typename COUT, typename T, typename M >
///@cond INTERNAL
requires quantity_concepts::can_be_printed_to< COUT, T >
///@endcond
COUT & operator<<( COUT & cout, const quantity< T, M > & right ){
   cout << '@';
   cout << ( (T)( right - quantity< T, M >() ) );
   return cout;
}
   
#endif // ifndef quantity_hpp