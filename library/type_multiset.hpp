// ==========================================================================
//
// type_multiset.hpp
//
// C++ header that implements a compile-time multiset of types
//
// https://www.github.com/wovo/quantity
// 
// Copyright Wouter van Ooijen - 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#ifndef type_multiset_hpp
#define type_multiset_hpp

// this file contains Doxygen lines
/// @file

// ==========================================================================
//
/// \page type_multiset
///
/// The type multiset is a type compile-time datastructure.
/// It is a type that maintains the number of times each type occurs in it
/// (its multiplicity).
/// It is used by the quantity ADT.
/// It lives in the namespace type_multiset.
///
/// The available functionality is:
///
/// type_multiset::empty
///    the empty type set (all elements have multiplicity 0)
///
/// type_multiset::one< typename T >
///    a multiset with only the type T with multiplicity 1
///
/// type_multiset::add< typename A, typename B >
///    adding two type multisets:
///    the result is a multiset in which for each element the 
///    multiplicity is the sum of its muliplicity in A and its 
///    multiplicity in B
///
/// type_multiset::multiply< typename A, int Factor >
///    multiplying a type multisest by an integer factor:
///    the result is a multiset in which for each element the
///    multiplicity is equal to Factor * its multiplicity in A
///
/// type_multiset::equal< typename A, typename B >
///    compare two multisets:
///    the result is true if and only if the multiplicity of each element
///    in the type multiset A is the same in the type multiset B, 
///    and vice versa
///
/// type_multiset::print< typename S, typename A >( S & stream )
///    when int, and each type T in A (with a non-zero multiplicity),
///    both have an operator<< for stream, this function prints the
///    typeset by calling, for each type T in a, 
///    the operator<< for an object of type T, and for its multiplicity.
///    Note that the order in which the types are printed is
///    undetermined.
///
//
// ==========================================================================


// doxygen only a above prose page is provided
///@cond INTERNAL

namespace type_multiset {

// ===========================================================================
//
// the datastructure
//
// The multiset is represented by a chain of nodes.
// The tail in a node is (points to) a next node.
// The last (sentinel) node is a dummy that contains ( void, o, void ).
// Hence an empty list has only that one node, the sentinel.
//
// The payload of each node is data (the type) 
// and count (its multiplicity).
//
// ===========================================================================

struct sentinel {
   using data                  = void;
   static constexpr int count  = 0;
   using tail                  = void;
};

template< typename Data, int Count, typename Tail = sentinel >
struct node {
   using data                  = Data;
   static constexpr int count  = Count;
   using tail                  = Tail;
};

using empty = sentinel;

template< typename T >
struct one : node< T, 1 >{};


// ===========================================================================
//
// add one element ( data + count )
//
// ===========================================================================

// add_element recursor: 
// re-create current node at the front, 
// recurse to add new node to the tail
template< 
   typename Data1, int Count1, 
   typename Data2, int Count2, typename Tail >
struct add_element_recursor : node < 
   Data2, 
   Count2, 
   add_element_recursor< 
      Data1, Count1, 
      typename Tail::data, Tail::count, 
      typename Tail::tail >>{};
   
// add_element recursion terminator: current node is the sentinel
// return the new node (with a sentinel after it)
template< typename Data1, int Count1 >
struct add_element_recursor < 
   Data1, Count1, 
   void, 0, void 
> : node < 
   Data1, 
   Count1 >{};

// add_element recursor: same element type
// merge into one node with added counts, keep tail as-is
template< typename Data1, int Count1, int Count2, typename Tail >
struct add_element_recursor < 
   Data1, Count1, 
   Data1, Count2, Tail
> : node < 
   Data1, 
   Count1 + Count2, 
   Tail >{};
   
// add_element interface: 
// unwrap the first node and call the add recursor
template< typename Data, int Count, typename List >
struct add_element : add_element_recursor < 
   Data, 
   Count, 
   typename List::data, 
   List::count, 
   typename List::tail >{};


// ===========================================================================
//
// prune
//
// remove nodes that have a count of 0
//
// Pruning is done whenever nodes with count 0 can be present.
// It keeps the size of the list in check,
// and makes printing and comparing easier.
//
// ===========================================================================

// prune recursor:
// re-create current node at the front, 
// recurse to add the pruned tail
template< typename Data, int Count, typename Tail >
struct prune2 : node <
   Data,
   Count,
   prune2 < 
      typename Tail::data, 
      Tail::count, 
      typename Tail::tail >>{};
   
// prune recursion terminator: current element is the sentinel
// return the (or rather a) sentinel
template<>
struct prune2< 
   void, 0, void 
> : 
   sentinel {};
  
// prune recursor: current element has a count of 0
// return the pruned tail  
template< typename Data, typename Tail >
struct prune2< 
   Data, 0, Tail 
> : prune2 < 
   typename Tail::data, 
   Tail::count, 
   typename Tail::tail >{};
   
// prune interface: 
// unwrap the first element and call prune recursor
template< typename List >
struct prune : prune2< 
   typename List::data,
   List::count,
   typename List::tail >{};


// ===========================================================================
//
// add two lists
//
// ===========================================================================

// add recursor: 
// add the current data to the 
// list to which the rest of the list is added recursively 
template< 
   typename Data, int Count, typename Tail,
   typename List > 
struct add_recursor : add_element < 
   Data, 
   Count, 
   add_recursor< 
      typename Tail::data, Tail::count, typename Tail::tail,
      List >>{};

// add recursion terminator: 
// when the current element is the sentinel, just return the (other) list
template< typename List > 
struct add_recursor<
   void, 0, void,
   List
> : List {};

// add interface: 
// unwrap the first node of the first list and call the add recursor
// prune the result
template< typename List1, typename List2 >
struct add : prune< add_recursor < 
   typename List1::data, 
   List1::count, 
   typename List1::tail,
   List2 >>{};
   
   
// ===========================================================================
//
// multiply
//
// multiply all node counts by a factor
//
// ===========================================================================

// multiply recursor:
// re-create current node at the front, 
// recurse to append the pruned tail
template< typename Data, int Count, int Factor, typename Tail >
struct multiply_recursor : node <
   Data,
   Count * Factor,
   multiply2 < 
      typename Tail::data, 
      Tail::count, Factor,
      typename Tail::tail >>{};
   
// multiply recursion terminator: current element is the sentinel
// return the (or rather a) sentinel
template< int Factor >
struct multiply_recursor < 
   void, 0, Factor, void 
> : 
   sentinel {};
    
// multiply interface: 
// unwrap the first element and call the recursor
template< typename List, int Factor >
struct multiply : multiply_recursor < 
   typename List::data,
   List::count,
   Factor,
   typename List::tail >{};   

   
// ===========================================================================
//
// equal
//
// check whether the multiplicities in both sets are all equal
//
// ===========================================================================

// equal tail recursor: 
// report whether the ( Data1, Count1 ) pair appears in the second list
template< 
   typename Data1, int Count1 
   typename Data2, int Count2, typename Tail >
struct equal_tail_recursor : equal_tail_recursor <
   Data1, Count1
   Tail::data, Tail::copunt, typename Tail::tail >{};
   
// equal tail match: 
// when both Data and Count match, report true
template< 
   typename Data, int Count, typename Tail >
struct equal_tail_recursor<
   Data, Count,
   void, 0, Tail
> {
   static constexpr bool value = true;
}   

// equal tail end-of-list: 
// when the end of the second list is reached, report false
template< 
   typename Data, int Count, typename Tail >
struct equal_tail_recursor<
   Data, Count,
   Data, Count, Tail
> {
   static constexpr bool value = false;
}   

// equal front recursor: 
// call the equal tail recursor to find whether ( Data1, Coun1 )
// appears in the second list,
// and recurse on the first list
template< 
   typename Data1, int Count1, typename Tail1, 
   typename Data2, int Count2, typename Tail2 >
struct equal_front_recursor : {
   
   using first = equal_tail_recursor <
      Data1, Count1,
      Data2, Count2, Tail2 >;
      
   using tail = equal_front_recursor <
      Tail1::data, Tail1::count, Tail1::tail,
      Data2, Count2, Tail2 >;
      
   static constexpr bool value = first::value && second::value;
};   

// equal interface: 
// unwrap the first element and call the recursor
template< typename List, typename Other >
struct equal {
   
   using forward = equal_front_recursor < 
      typename List::data,  List::count,  typename List::tail,
      typename Other::data, Other::count, typename Other::tail >{};   
      
   using backward = equal_front_recursor <     
      typename Other::data, Other::count, typename Other::tail,
      typename List::data,  List::count,  typename List::tail >{};
      
   static constexpr bool value = forward::value && backward::value;   
};   
   
} // namespace type_multiset
   
///@endcond // INTERNAL   
   
#endif // #ifndef type_multiset_hpp