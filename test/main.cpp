#include <iostream>

/*
 * add a factor for -1
 * check added to a pruned list
 * test with catch2? print to string
 * check for equivalent/same
*/

// ===========================================================================
//
// list
//
// A list is a chain of nodes, each tail is (points to) a next node.
// The last (sentinel) node is a dummy.
// Hence an empty list has one node, the sentinel.
//
// ===========================================================================

struct sentinel {
   using element               = void;
   static constexpr int count  = 0;
   using tail                  = void;
};

template< typename Element, int Count, typename Tail = sentinel >
struct node {
   using element               = Element;
   static constexpr int count  = Count;
   using tail                  = Tail;
};


// ===========================================================================
//
// add one element
//
// ===========================================================================

// add_element recursor: 
// re-create current node at the front, 
// recurse to add new node to the tail
template< 
   typename Element1, int Count1, 
   typename Element2, int Count2, typename Tail >
struct add_element_recursor : node < 
   Element2, 
   Count2, 
   add_element_recursor< 
      Element1, Count1, 
      typename Tail::element, Tail::count, 
      typename Tail::tail >>{};
   
// add_element recursor: current node is the sentinel
// return the new node (with a sentinel after it)
template< typename Element1, int Count1 >
struct add_element_recursor < 
   Element1, Count1, 
   void, 0, void 
> : node < 
   Element1, 
   Count1 >{};

// add_element recursor: same element type
// merge into one node with added counts, keep tail as-is
template< typename Element1, int Count1, int Count2, typename Tail >
struct add_element_recursor < 
   Element1, Count1, 
   Element1, Count2, Tail
> : node < 
   Element1, 
   Count1 + Count2, 
   Tail >{};
   
// add_element interface: 
// unwrap the first node and call the add recursor
template< typename Element, int Count, typename List >
struct add_element : add_element_recursor < 
   Element, 
   Count, 
   typename List::element, 
   List::count, 
   typename List::tail >{};


// ===========================================================================
//
// prune
//
// remove nodes that have a count of 0
//
// ===========================================================================

// prune recursor:
// re-create current node at the front, 
// recurse to add the pruned tail
template< typename Element, int Count, typename Tail >
struct prune2 : node <
   Element,
   Count,
   prune2 < 
      typename Tail::element, 
      Tail::count, 
      typename Tail::tail >>{};
   
// prune recursor: current element is the sentinel
// return the (or rather a) sentinel
template<>
struct prune2< 
   void, 0, void 
> : 
   sentinel {};
  
// prune recursor: current element has a count of 0
// return the pruned tail  
template< typename Element, typename Tail >
struct prune2< 
   Element, 0, Tail 
> : prune2 < 
   typename Tail::element, 
   Tail::count, 
   typename Tail::tail >{};
   
// prune interface: 
// unwrap the first element and call prune recursor
template< typename List >
struct prune : prune2< 
   typename List::element,
   List::count,
   typename List::tail >{};


// ===========================================================================
//
// add two lists
//
// ===========================================================================

// add recursor: 
// add the current element to the 
// list to which the rest of the list is added recursively 
template< 
   typename Element, int Count, typename Tail,
   typename List > 
struct add_recursor : add_element < 
   Element, 
   Count, 
   add_recursor< 
      typename Tail::element, Tail::count, typename Tail::tail,
      List >>{};

// add recursor: 
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
   typename List1::element, 
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
template< typename Element, int Count, int Factor, typename Tail >
struct multiply2 : node <
   Element,
   Count * Factor,
   multiply2 < 
      typename Tail::element, 
      Tail::count, Factor,
      typename Tail::tail >>{};
   
// multiply recursor terminator: current element is the sentinel
// return the (or rather a) sentinel
template< int Factor >
struct multiply2< 
   void, 0, Factor, void 
> : 
   sentinel {};
    
// multiply interface: 
// unwrap the first element and call the recursor
template< typename List, int Factor >
struct multiply : multiply2 < 
   typename List::element,
   List::count,
   Factor,
   typename List::tail >{};   


// ===========================================================================
//
// test & print
//
// ===========================================================================

template< char C >
struct tag { static constexpr char c = C; };

template< typename Element, int Count, typename Tail >
void print2(){
   std::cout << Element::c << Count << " ";
   print2< 
      typename Tail::element, 
      Tail::count, 
      typename Tail::tail >();
}

template<>
void print2< void, 0, void >(){
   std::cout << "!\n ";
}

template< typename T >
void print(){
   print2< 
      typename T::element, 
      T::count, 
      typename T::tail >();
}



using list1 = node< tag< 'A' >, 1, sentinel >;
using list2 = node< tag< 'B' >, 2, list1 >;

int main(int argc, char **argv){
   print< sentinel >();                            // !
   print< list1 >();                            // A1
   print< list2 >();                            // B2 A1
   print< add_element< tag< 'A' >, 5, sentinel >>();      // A5
   print< add_element< tag< 'B' >, 5, sentinel >>();      // B5
   print< add_element< tag< 'A' >, 5, list1 >>();      // A6
   print< add_element< tag< 'B' >, 5, list1 >>();      // B5 A1
   print< add_element< tag< 'A' >, 5, list2 >>();      // B2 A6
   print< add_element< tag< 'B' >, 5, list2 >>();      // B7 A1
   print< prune< sentinel >>();                    // !
   print< prune< list1 >>();                    // A1
   print< prune< list2 >>();                    // B2 A1
   print< add_element< tag< 'B' >, -2, list2 >>();      // B0 A1
   print< prune< add_element< tag< 'B' >, -2, list2 >>>();      // A1
   print< add_element< tag< 'A' >, -1, list2 >>();      // B2 A0
   print< prune< add_element< tag< 'A' >, -1, list2 >>>();      // B2
   print< add_element< tag< 'A' >, -1, list1 >>();      // A0
   print< prune< add_element< tag< 'A' >, -1, list1 >>>();      // !
   
   print< add< list1, list2 > >();
   print< add< add< list1, list2 >, node< tag< 'A' >, -2 > > >();
   print< add< add< list1, list2 >, node< tag< 'B' >, -2 > > >();
   print< add< 
      add< add< list1, list2 >, node< tag< 'B' >, -2 > >,
      node< tag< 'A' >, -2 >
      > >();

}
