#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>
#include <malloc.h>

typedef uint32_t KeyType;
typedef uint32_t InfoType;
typedef uint32_t IndexType;
typedef uint32_t RelType;

typedef struct Node {
    RelType release;
    InfoType info;
    struct Node *next;
 } Node;

typedef struct KeySpace {
    KeyType key;
    Node *node;
    struct KeySpace *next;
} KeySpace;

typedef struct Table {
    KeySpace **ks;
    IndexType msize;
    IndexType csize;
} Table;

int createTable(IndexType msize, Table** __out);
KeySpace* find(Table* table, KeyType key);
Node* findRelease(Table* table, KeyType key, RelType release);
int findAllVersions(Table* table, InfoType** __out_array, KeyType key);
int insert(Table* table, KeyType key, InfoType info);
int deleteByRelease(Table* table, KeyType key, RelType release);
int deleteHeadRelease(Table* table, KeyType key);
int deleteKeySpace(Table* table, KeyType key);
int individualDelete(Table* table);
int clearTable(Table *table);
int freeTable(Table** table);

#endif