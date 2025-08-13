#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "return_code.h"

/**
 * @return 0 - succes(SUCCESS), -2 - invalid first argument(INVALID_ARGUMENT(1)) -3 - invalid second argument
 *          -1 - not memory
 */
int create(int msize, Table** __out){
    if (msize < 0) return INVALID_ARGUMENT(1);
    if (__out == NULL) return INVALID_ARGUMENT(2);

    Table* table = malloc(sizeof(Table));
    if(table == NULL) return NOT_ENOUTH_MEMORY;
    table->ks = calloc(msize, sizeof(KeySpace));
    if(table->ks == NULL) return NOT_ENOUTH_MEMORY;
    table->msize = msize;
    table->csize = 0;

    *__out = table;
    return SUCCESS;
}

int hash(int key, int msize) {
    return key % msize;
}

KeySpace* find(Table* table, int key) {
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    while(ptr->next != NULL) {
        if(ptr->key == key) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

int findAllVersions(Table* table, int key);


int insert(Table* table, int key, int info) {         //нет условия для увеличения csize
    if(table->msize == 0 || table == NULL) return 1;
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    KeySpace* prev = NULL;
    while(ptr != NULL) {
        if(ptr->key == key) {
            Node* newnode = malloc(sizeof(Node));
            if(newnode == NULL) return 2;
            newnode->release = ++ptr->node->release;
            newnode->info = info;
            newnode->next = ptr->node;
            ptr->node = newnode;
            return 0;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    KeySpace* new = malloc(sizeof(KeySpace));
    if(new == NULL) return 2;
    new->key = key;
    new->node->info = info;
    new->node->release = 1;
    if(prev == NULL) {
        ++table->csize;
        new->next = NULL;
        table->ks[index] = new;

    }
    else {
        new->next = table->ks[index];
        table->ks[index] = new;
    }
    return 0;
}

int delete(Table* table, int key, int release) {
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    KeySpace* prev = NULL;
    while(ptr->next != NULL) {
        if(ptr->key == key) {
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    
}

int seeTable(Table* table) {
    if(table->csize == 0) {
        printf("Table is empty\n");
        return 1;
    }
    printf("********TABLE**********\n");
    for(int i = 0; i != table->msize; i++) {
        printf("Keyspace %d:\n", i);
        KeySpace* ptr = table->ks[i];
        while(ptr->next != NULL) {
            printf("    Key->%d\n", ptr->key);
            while(ptr->node->next != NULL) {
                printf("        Release->%d   Info->%d\n", ptr->node->release, ptr->node->info);
                ptr->node = ptr->node->next;
            }
            ptr = ptr->next;
        }
    }
    return 0;
}

int import(Table* table, const char* filename) {

}

int export(Table* table, const char* filename) {

}

int individualDelete(Table* table);

void freeTable(Table* table) {
    
}

