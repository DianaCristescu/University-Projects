#include <stdio.h>
#include <stdlib.h>

typedef void T;

typedef struct node {
    T *payload;
    struct node *next; 
} Node;

typedef struct list {
    Node *head;
} List;

List* list_init() {
    List *list = (List*)malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void list_destroy(List *list) {
    Node *el = list->head;
    Node *tmp = NULL;

    while(el) {
        tmp = el->next;
        free(el->payload);
        free(el);
        el = tmp;
    }
    free(list);

}

void raise(char *msg) {
    printf("EXCEPTION: %s", msg);
    exit(1);
}

void list_insert(List *list, int pos, T *el) {
    if(list == NULL) raise("undefined list");
    if(list->head == NULL && pos > 0) raise("insert past end");

    if(pos == 0) {
        Node *new_node = (Node*)malloc(sizeof(Node));
        new_node->payload = el;
        new_node->next = list->head;
        list->head = new_node;
    } else {

        Node *prev = NULL;
        Node *crt = list->head;
        int crt_pos = 0;

        while(crt && crt_pos != pos) {
            prev = crt;
            crt = crt->next;
            crt_pos++;
        }
        if(prev == NULL) raise("prev is NULL");
        if(crt == NULL && crt_pos != pos) raise("insert past end");

        Node *new_node = (Node*)malloc(sizeof(Node));
        new_node->payload = el;
        new_node->next = crt;
        prev->next = new_node;
    }
} 

T* list_get(List *list, int pos) {
    if(list == NULL) raise("undefined list");
    if(list->head == NULL && pos > 0) raise("get past end");

    Node *crt = list->head;
    int crt_pos = 0;

    while(crt && crt_pos != pos) {
        crt = crt->next;
        crt_pos++;
    }

    if(crt == NULL || crt_pos != pos) raise("get past end");
    return crt->payload;
}

int main() {

    int a = 10;
    int b = 20;
    int c = 30;

    List *list = list_init();
    list_insert(list, 0, &a);
    list_insert(list, 0, &b);
    list_insert(list, 2, &c);
    printf("\nrez = %d\n", *(int*)list_get(list, 0));
    printf("\nrez = %d\n", *(int*)list_get(list, 1));
}