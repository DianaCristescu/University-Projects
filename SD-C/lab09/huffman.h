#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <string.h>

typedef struct {
    unsigned char value;
    int left, right;
} HuffmanNode;

#define Item int

// Including below so `Item` is defined.
#include "heap.h"

//---------------------------------------------------------------------------------*

void computeFreqs(char *text, int size, int freqs[256]) {
 
    for(int i=0; i<256; i++)
        freqs[i] = 0;

    for (int i=0; i<size; i++) {
        int k = (int)text[i];
        freqs[k] += 1;
    }

    // for(int i=0; i<256; i++)
    //     if(freqs[i] != 0)
    //         printf("%c: %d\n", (char)i, freqs[i]);

}

//---------------------------------------------------------------------------------*

HuffmanNode createHuffmanNode(unsigned char value, int left, int right) {
    HuffmanNode node;
    node.value = value;
    node.left = left;
    node.right = right;
    return node;
}

ItemType createItem(int freq, Item index) {
    ItemType item;
    item.prior = freq;
    item.content = index;
    return item;
}

//---*

HuffmanNode *makeTree(int freqs[256], int *size) {

    HuffmanNode *huffman = (HuffmanNode*)malloc(500*sizeof(HuffmanNode));
    APriQueue heap = makeQueue(500);
    int k = 0;
    ItemType item;

    for(int i=0; i<256; i++) {
        if(freqs[i] != 0) {
            huffman[k] = createHuffmanNode((char)i, -1, -1);
            item = createItem(freqs[i], k);
            insert(heap, item);
            k++;
        }
    }

    *size = k;
    ItemType left, right;
    while(heap->size >= 2) {
        left = heap->elem[0];
        right = heap->elem[1];
        huffman[k] = createHuffmanNode('*', left.content, right.content);

        item = createItem(left.prior + right.prior, k);
        insert(heap, item);

        removeMin(heap);
        removeMin(heap);
        k++;
    }

    *size = k;
    free(heap);
    return huffman;
}

//---------------------------------------------------------------------------------*

void makeCodes(int index, HuffmanNode *nodes, char **codes, char *code) {

    int l = 0;
    int r = 0;
    
    int idx_left = (nodes[index]).left;
    if(idx_left != -1) {
        HuffmanNode left = nodes[idx_left];
        if(left.left == -1 && left.right == -1) {
            strcat(code, "0\0");
            codes[(int)left.value] = code;
            // strcat(*codes + (int)left.value, code);
            strcpy(code, "");
            l = 1;
        }
    }
    
    int idx_right = (nodes[index]).right;
    if(idx_right != -1) {
        HuffmanNode right = nodes[idx_right];
        if(right.left == -1 && right.right == -1) {
            strcat(code, "1\0");
            codes[(int)right.value] = code;
            // strcat(*codes + (int)right.value, code);
            strcpy(code, "");
            r = 1;
        }
    }

    if(l && r) return;
    else if(l) {
        strcat(code, "1");
        makeCodes(idx_right, nodes, codes, code);
    } else if(r) {
        strcat(code, "0");
        makeCodes(idx_left, nodes, codes, code);
    } else {
        char *code2 = (char*)malloc(500*sizeof(char));
        strcpy(code2, code);

        strcat(code, "0");
        makeCodes(idx_left, nodes, codes, code);

        strcat(code2, "1");
        makeCodes(idx_right, nodes, codes, code2);
    }

}

//---------------------------------------------------------------------------------*

char *compress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {

    char **codes = (char**)malloc(256*sizeof(char*));
    char *code = (char*)malloc(500*sizeof(char));
    makeCodes(numNodes-1, nodes, codes, code);

    char *compressed = (char*)malloc(100*500*sizeof(char));
    strcpy(code, "");

    for(int i=0; i<textSize; i++) {
       code = codes[text[i]];
       strcat(compressed, code);
    }

    return compressed;
}

//---------------------------------------------------------------------------------*

char *decompress(char *text, int textSize, HuffmanNode *nodes, int numNodes) {
    return NULL;
}

//---------------------------------------------------------------------------------*

#endif
