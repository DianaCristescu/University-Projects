#include <stdio.h>
#include <math.h>
typedef long T;
#include "OrderedSet.h"

OrderedSet *multiples(long x, double interval[3])
{
    OrderedSet *set = createOrderedSet(interval[1]-interval[0]);
    for(long i=(long)ceil(interval[0]/x); i<=(long)floor(interval[1]/x); i++)
        add(set, i*x);
    return set;
}

void print(OrderedSet *set)
{
    for(int i=0; i<set->size; i++)
        printf("%ld ", set->elements[i]);
    printf("\n");
}

int main()
{
    double interval1[2] = {4, 25};
    double interval2[2] = {5, 30};
    OrderedSet *set1 = multiples(3l, interval1);
    OrderedSet *set2 = multiples(4l, interval2);
    OrderedSet *union_set = unionOrderedSets(set1, set2);
    OrderedSet *intersection_set = intersectOrderedSets(set1, set2);
    printf("There are 7 elements: ");
    print(set1);
    printf("There are 6 elements: ");
    print(set2);
    printf("There are 11 elements: ");
    print(union_set);
    printf("There are 2 elements: ");
    print(intersection_set);
}