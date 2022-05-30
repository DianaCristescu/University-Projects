#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* Key;
typedef int Value;
typedef long(*HashFunction)(Key, long);

typedef struct Element {
  Key key;
  Value value;
  struct Element *next;
} Element;

typedef struct HashTable {
  Element** elements;
  long size;
  HashFunction hashFunction;
} HashTable;

void initHashTable(HashTable **h, long size, HashFunction f) {
  // Cerinta 1
  (*h) = (HashTable*)malloc(sizeof(HashTable));
  (*h)->elements = (Element**)malloc(size*sizeof(Element*));
  for(int i=0; i<size; i++)
    (*h)->elements[i] = NULL;
  (*h)->size = size;
  (*h)->hashFunction = f;
}

int exists(HashTable *hashTable, Key key) {
  // Cerinta 1
  int idx = hashTable->hashFunction(key, hashTable->size);
  if(hashTable->elements[idx] == NULL)
    return 0;
  else {
    for(Element *el=hashTable->elements[idx]; el!=NULL; el=el->next) {
      if(!strcmp(el->key, key)) return 1;
    }
    return 0;
  }
}

Value get(HashTable *hashTable, Key key) {
  // Cerinta 1
  int idx = hashTable->hashFunction(key, hashTable->size);
  if(hashTable->elements[idx] == NULL)
    return (Value)0;
  else {
    for(Element *el=hashTable->elements[idx]; el!=NULL; el=el->next) {
      if(!strcmp(el->key, key)) return el->value;
    }
    return (Value)0;
  }
}

void put(HashTable *hashTable, Key key, Value value) {
  // Cerinta 1
  int idx = hashTable->hashFunction(key, hashTable->size);
  Element **p = &(hashTable->elements[idx]);

  if(exists(hashTable, key)) {
    while((*p) != NULL) {
      if(!strcmp((*p)->key, key)) {
        (*p)->value += value;
        break;
      }
      p = &((*p)->next);
    }

  }else {
    Element *el = (Element*)malloc(sizeof(Element));
    el->key = (char*)malloc(50*sizeof(char));
    el->key = strcpy(el->key, key);
    el->value = value;
    el->next = NULL;

    if((*p) == NULL)
      (*p) = el;
    else {
      while((*p)->next != NULL)
        p = &((*p)->next);
      (*p)->next = el; 
    }
  }

}

void deleteKey(HashTable *hashTable, Key key) {
  // Cerinta 1
  int idx = hashTable->hashFunction(key, hashTable->size);

  if(!exists(hashTable, key))
    return;

  else {
    for(Element *el=hashTable->elements[idx]; el!=NULL; el=el->next) {
      if(!strcmp(el->next->key, key)) {
        Element *next = el->next->next;
        free(el->next->key);
        free(el->next);
        el->next = next;
        return;
      }
    }
  }

}

void print(HashTable *hashTable) {
  // Cerinta 1
  Element *el = NULL;
  for(int i=0; i<hashTable->size; i++) {
    el = hashTable->elements[i];
    if(el != NULL) {
      printf("%d:\n", i);
      for(Element *p=el; p!=NULL; p=p->next)
        printf("\t%s : %d\n", p->key, p->value);
    }
  }
}

void freeHashTable(HashTable *hashTable) {
  // Cerinta 1
  Element *next = NULL;
  Element *el = NULL;
  for (int i=0; i<hashTable->size; i++) {
    el = hashTable->elements[i];
    while(el != NULL) {
      next = el->next;
      free(el->key);
      free(el);
      el = next;
    }
  }
  free(hashTable->elements);
  free(hashTable);
}


long hash1(Key word, long size) {
  // Cerinta 2
  long h = 0l;
  for(int i=0; i<strlen(word); i++)
    h = h * 17l + (long)word[i];
  return h % size;
}

int min(int a, int b) {
  if(a<b) return a;
  else return b;
}

int main(int argc, char* argv[]) {
  HashTable *hashTable1;
  FILE *f1, *f2;
  char word[256];
  long hashSize, common;

  hashSize = atoi(argv[1]);
  f1 = fopen(argv[2], "r");
  f2 = fopen(argv[3], "r");

  initHashTable(&hashTable1, hashSize, &hash1);

  // Cerinta 3
  while(fscanf(f1, "%s ", word) != EOF) {
    put(hashTable1, word, 1);
  }

  print(hashTable1);
  // ...

  // Cerinta 4
  HashTable *hashTable2;
  initHashTable(&hashTable2, hashSize, &hash1);
  while(fscanf(f2, "%s ", word) != EOF) {
    put(hashTable2, word, 1);
  }

  common = 0l;
  int c1, c2, c; //appereance count for hashTable 1&2, final number of appereances
  for(int i=0; i<hashTable1->size; i++)
    for(Element *el=hashTable1->elements[i]; el!=NULL; el=el->next) {

      if(exists(hashTable2, el->key)) {

        if(common == 0l)
          printf("\nCommon words:\n\n");

        c1 = el->value;
        c2 = get(hashTable2, el->key);
        c = min(c1, c2);

        common++;
        printf("\t%s : %d\n", el->key, c);
      }
    }
  // ...


  freeHashTable(hashTable1);
  freeHashTable(hashTable2);
  printf("\ncount: %ld\n", common);

  fclose(f1);
  fclose(f2);
  return 0;
}
