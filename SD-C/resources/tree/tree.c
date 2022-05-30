#include <stdio.h>

typedef void T; 

typedef struct treeNode {
    T *payload;
    struct treeNode* parent;
    struct treeNode** children;
} TNode;