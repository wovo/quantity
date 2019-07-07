   /// divide a quantity by another quantity
   ///
   /// Divide ourself by another quantity.
   /// The base types of our quantity and the value must be dividable.
   /// The result is a quantity of the type 
   /// and with the value of that division.
   template< typename X, typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_divided_with_value< X, W >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator/( 
      const X & left,
      const quantity_implementation< W, U > & right
   ){
      return right._reverse_divide( left );
   }
   
   /// multiply a plain value with a quantity
   ///
   /// Multiply a plain value with ourself.
   /// The base types of the value and the value of our quantity 
   /// must be multiplyable.
   /// The result is a quantity of the type 
   /// and with the value of that multiplication.
   template< typename X, typename W, typename U >
   ///@cond INTERNAL
   requires quantity_concepts::can_be_multiplied_with_value< X, W >
   __attribute__((always_inline))
   ///@endcond
   constexpr auto operator*( 
      const X & left, 
      const quantity_implementation< W, U > & right
   ){
      return right._reverse_multiply( left );    
   }   