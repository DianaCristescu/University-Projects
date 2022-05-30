#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
    IMPORTANT!

    As we stick to pure C, we cannot use templates. We will just asume
    some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct node {
    T value;
    struct node* next;
} TNode, *TSortedList;

// TODO : Cerința 1 & Cerința 2

TSortedList create(T value) {
    TSortedList newList = NULL;
    newList = (TSortedList)malloc(sizeof(TNode));
    newList->value = value;
    newList->next = NULL;
    return newList;
}

int isEmpty(TSortedList list) {
    if(list == NULL)
        return 1;
    else 
        return 0;
}

int contains(TSortedList list, T value) {
    while(!isEmpty(list)) {
        if(list->value == value)
            return 1;
        list = list->next;
    }
    return 0;
}

TSortedList insert(TSortedList list, T value) {
    if(isEmpty(list)) {
        list = create(value);
        return list;
    }

    TSortedList node=list, prev_node=NULL;
    int true = 1;
    while(!isEmpty(node->next) && true) {
        if(value <= node->value)
            true = 0;
        if(true) {
            prev_node = node;
            node = node->next;
        }
    }

    if(isEmpty(node->next) && (value <= node->value))
        true = 0;

    TSortedList newNode = create(value);

    if(true) {                       //true => trebuie inserat la sfarsit
        node->next = newNode;
        return list;
    } else if(!isEmpty(prev_node)) {  //!isEmpty(prev_node) => trebuie inserat in interiorul listei
        prev_node->next = newNode;
        newNode->next = node;
        return list;
    } else {                          // => trebuie inserat la inceput
        newNode->next = node;
        return newNode;
    }
}

TSortedList deleteOnce(TSortedList list, T value) {
    if(isEmpty(list)) {
        return list;
    }

    TSortedList node=list, prev_node=NULL;
    int true = 1;
    while(!isEmpty(node->next) && true) {
        if(value == node->value)
            true = 0;
        if(true) {
            prev_node = node;
            node = node->next;
        }
    }

    if(isEmpty(node->next) && (value == node->value))
        true = 0;

    if(true) {
        prev_node->next = NULL;
        free(node);
        return list;
    } else if(!isEmpty(prev_node)) {
        prev_node->next = node->next;
        free(node);
        return list;
    } else {
        if(isEmpty(node->next)) {
            free(list);
            return NULL;
        } else {
            prev_node = node->next;
            free(node);
            return prev_node;
        }
    }

}

long length(TSortedList list) {
    long i = 0;
    while(!isEmpty(list)) {
        list = list->next;
        i++;
    }
    return i;
}

T getNth(TSortedList list, int n) {
    int i = 0;
    while(!isEmpty(list)) {
        i++;
        if(i == n)
            return list->value;
        list = list->next;
    }
    return 0;
}

TSortedList freeTSortedList(TSortedList list) {
    TSortedList tmp;
    while(!isEmpty(list)) {
        tmp = list;
        list = list->next;                                                                     
        free(tmp);
    }
    return NULL;
}

#endif
