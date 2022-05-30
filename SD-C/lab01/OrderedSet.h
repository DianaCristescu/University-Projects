#ifndef ORDERED_SET_H
#define ORDERED_SET_H

#include <stdio.h>                                          // needed for printf
#include <stdlib.h>                           // needed for alloc, realloc, free
#include <stdlib.h>

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------

/*
  OrderedSet represents a set with elements of type T stored in memory
  in an ordered fashion.
*/

typedef struct OrderedSet
{
  T* elements;                    // pointer to where the elements are in memory
  long size;                                // the number of elements in the set
  long capacity;                      // the current capacity (allocated memory)
} OrderedSet;

// -----------------------------------------------------------------------------

/*
  Function that creates an OrderedSet with elements of type T with a given
  initial capacity.
*/

OrderedSet* createOrderedSet(long initialCapacity) 
{
  OrderedSet* newSet = (OrderedSet*) malloc(sizeof(OrderedSet));
  newSet->size = 0;                        // initially, the set is empty
  newSet->capacity = initialCapacity;
  newSet->elements = (T*) malloc(initialCapacity * sizeof(T));
  return newSet;
}

// -----------------------------------------------------------------------------

/*
  Function that checks if an element exists in a given set. As the
  elements of the set are ordered in memory, this function uses binary
  search.

  Function returns 1 if the given element exists in the set, and 0
  otherwise.
*/

int contains(OrderedSet* set, const T element) 
{
  // TODO : Cerința 2
  for(int i=0; i<set->size; i++)
    if(set->elements[i] == element) return 1;
  return 0;
}

// -----------------------------------------------------------------------------

/*
  Function that adds a new element to a set. If the element was
  already in the set, nothing changes. Otherwise, the element is added
  to the set. In the end, the elements must be in ascending order.
*/

void add(OrderedSet* set, const T newElement) 
{
  // TODO : Cerința 2
  T replace = newElement;
  T aux = ' ';
  for(int i=0; i<=set->size; i++)
  {
    if(set->elements[i] == replace) return;
    else if(i == set->size) set->elements[i] = replace;
    else if(set->elements[i] > replace)
    {
      aux = set->elements[i];
      set->elements[i] = replace;
      replace = aux;
    }
  }
  set->size++;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the union of
  the two
*/

OrderedSet* unionOrderedSets(OrderedSet* s1, OrderedSet* s2) 
{
  // TODO : Cerința 3
  OrderedSet *union_set = NULL;
  union_set = createOrderedSet(s1->capacity+s2->capacity);
  for(int i=0; i<s1->size; i++)
    add(union_set, s1->elements[i]);
  for(int i=0; i<s2->size; i++)
    add(union_set, s2->elements[i]);
  return union_set;
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the
  intersection of the two
*/

OrderedSet* intersectOrderedSets(OrderedSet* s1, OrderedSet* s2) 
{
  // TODO : Cerința 3
  OrderedSet *intersection_set = NULL;
  if(s1->size <= s2->size)
  {
      intersection_set = createOrderedSet(s1->size);
      for(int i=0; i<s1->size; i++)
        if(contains(s2, s1->elements[i]))
          add(intersection_set, s1->elements[i]);
  }
  else
  {
      intersection_set = createOrderedSet(s2->size);
      for(int i=0; i<s2->size; i++)
        if(contains(s1, s2->elements[i]))
          add(intersection_set, s2->elements[i]);
  }
  return intersection_set;
}


// -----------------------------------------------------------------------------

#endif
