#ifndef TABLE_H
#define TABLE_H

#include "stdint.h"

typedef struct Node {
    int release;
    int info;
    struct Node *next;
 }Node;

typedef struct KeySpace {
    int key;
    Node *node;
    struct KeySpace *next;
} KeySpace;

typedef struct Table {
    KeySpace **ks;
    int msize;
    int csize;
} Table;

int create(int msize, Table** __out);
KeySpace* find(Table* table, int key);
Node* findRelease(Table* table, int key, int release);
int findAllVersions(Table* table, Node* __out, int key);
int insert(Table* table, int key, int info);
int deleteByRelease(Table* table, int key, int release);
int deleteHeadRelease(Table* table, int key);
int deleteKeySpace(Table* table, int key);
int seeTable(Table* table);
int individualDelete(Table* table);
int freeTable(Table* table);

#endif