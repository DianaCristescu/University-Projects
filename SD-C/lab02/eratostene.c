#include <stdio.h>

typedef int T;
#include "SortedList.h"

TSortedList getNaturals(int A, int B) {
    TSortedList nats = NULL;
    int i;
    for (i = A; i <= B; i++) {
        nats = insert(nats, i);
    }
    return nats;
}

TSortedList getPrimes(int N) {
    TSortedList nats = getNaturals(2, N);

    TSortedList primeNode = nats;
    TSortedList nodeDelete;
    TSortedList temp;

    while(!isEmpty(primeNode)) {
        nodeDelete = primeNode->next;

        while(!isEmpty(nodeDelete)) {
            if(nodeDelete->value % primeNode->value == 0) {
                temp = nodeDelete;
                nodeDelete = nodeDelete->next;
                deleteOnce(nats, temp->value);
            } else
                nodeDelete = nodeDelete->next;
        }

        primeNode = primeNode->next;
    }

    return nats;
}

void printInts(TSortedList list) {
    while (!isEmpty(list)) {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    printInts(getPrimes(100));
    return 0;
}
