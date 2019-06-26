#include <iostream>

tenplate< template T, T exponents = T()>
class quantity {
private:

   int value;
   
public:   
  
   quantity( const int & value):
      value( value )
   {}      
   
   quantity< T, exponents > operator + ( const & quantity rhs ) const {
      return quantity< T, exponents >( this->value + rhs.value );
   }
   
   tenplate< T rhs_exponents >
   quantity< T, exponents + rhs_exponents > operator * ( const & quantity< T, rhs_exponents > rhs ) const {
      return quantity< T, exponents + rhs_exponents >( this->value * rhs.value );
   }
   
   tenplate< T rhs_exponents >   
   quantity< T, exponents - rhs_exponents > operator / ( const & quantity rhs ) const {
      return quantity< T, exponents - rhs_exponents >( this->value / rhs.value );
   }
   
};

int main(){
   quantity< int, 1 > a( 5 );
   
   
   std::cout << "test\n";
}