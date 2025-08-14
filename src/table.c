#include <stdio.h>
#include <stdlib.h>
#include "include/table.h"
#include "include/return_code.h"

/**
 * @return 0 - succes(SUCCESS), -2 - invalid first argument(INVALID_ARGUMENT(1)) -3 - invalid second argument
 *          -1 - not memory
 */
int create(int msize, Table** __out){
    if (msize < 0) return INVALID_ARGUMENT;
    if (__out == NULL) return INVALID_ARGUMENT;

    Table* table = malloc(sizeof(Table));
    if(table == NULL) return ERROR_OF_MEMORY;
    table->ks = calloc(msize, sizeof(KeySpace));
    if(table->ks == NULL) return ERROR_OF_MEMORY;
    table->msize = msize;
    table->csize = 0;

    *__out = table;
    return SUCCESS;
}

int hash(int key, int msize) {
    return key % msize;
}

Node* findRelease(Table* table, int key, int release) {
    if (table == NULL || table->ks == NULL) return TABLE_IS_EMPTY;
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    while (ptr != NULL) {
        if(ptr->key == key) break;
        ptr = ptr->next;
    }
    if (ptr == NULL) return ELEMENT_NOT_FOUND;
    Node* ptr_node = ptr->node;
    while (ptr_node != NULL) {
        if(ptr_node->release == release) return ptr_node;
        ptr_node = ptr_node->next;
    }
    if (ptr_node == NULL) return ELEMENT_NOT_FOUND;
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

int findAllVersions(Table* table, Node* __out, int key) {
    KeySpace* ptr = find(table, key);
    if(ptr == NULL) return ELEMENT_NOT_FOUND;
    __out = malloc(ptr->node->release * sizeof(Node));
    if(__out == NULL) return ERROR_OF_MEMORY;
    int n = ptr->node->release;
    for(int index = 0; index != n; index++) {
        __out[index].info = ptr->node->info;
        __out[index].release = ptr->node->release;
        __out[index].next = NULL;
        ptr->node = ptr->node->next;
    }
    return SUCCESS;
}


int insert(Table* table, int key, int info) {
    if(table->msize == 0 || table == NULL) return TABLE_IS_EMPTY;
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    KeySpace* prev = NULL;
    while(ptr != NULL) {
        if(ptr->key == key) {
            Node* newnode = malloc(sizeof(Node));
            if(newnode == NULL) return ERROR_OF_MEMORY;
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
    if(new == NULL) return ERROR_OF_MEMORY;
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
    return SUCCESS;
}


int deleteByRelease(Table* table, int key, int release) {
    if (table == NULL || table->ks == NULL) return TABLE_IS_EMPTY;
    int index = hash(key, table->msize);
    KeySpace* prev = NULL;
    KeySpace* ptr = table->ks[index];
    while (ptr != NULL) {
        if(ptr->key == key) break;
        prev = ptr;
        ptr = ptr->next;
    }
    if (ptr == NULL) return ELEMENT_NOT_FOUND;
    Node* prev_node = NULL;
    Node* ptr_node = ptr->node;
    while (ptr_node != NULL) {
        if(ptr_node->release == release) break;
        prev_node = ptr_node;
        ptr_node = ptr_node->next;
    }
    if (ptr_node == NULL) return ELEMENT_NOT_FOUND;
    if (prev_node == NULL) {
        ptr->node = ptr_node->next;
    } else {
        prev_node->next = ptr_node->next;
    }
    free(ptr_node);
    if (ptr->node == NULL) {
        if (prev == NULL) {
            table->ks[index] = ptr->next;
        } else {
            prev->next = ptr->next;
        }
        free(ptr);
    }
    return SUCCESS;
}


int deleteHeadRelease(Table* table, int key) {
    if (table == NULL || table->ks == NULL) return TABLE_IS_EMPTY;
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    KeySpace* prev = NULL;
    while(ptr != NULL) {
        if(ptr->key == key) break;
        prev = ptr;
        ptr = ptr->next;
    }
    if(ptr == NULL) return ELEMENT_NOT_FOUND;
    Node* tmp =  ptr->node;
    if(tmp->next != NULL) {
        Node* next = tmp->next;
        free(tmp);
        ptr->node = next;
    }
    else { //prev != NULL не придумал как обработать
        KeySpace* next = ptr->next;
        free(ptr->node);
        free(ptr);
        prev->next = next;
    }
    return SUCCESS;
}

int deleteKeySpace(Table* table, int key) {
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    KeySpace* prev = NULL;
    while(ptr != NULL) {
        if(ptr->key == key) break;
        prev = ptr;
        ptr = ptr->next;
    }
    if(ptr == NULL) return ELEMENT_NOT_FOUND;
    Node *node = ptr->node;
    while (node != NULL) {
        Node *temp = node;
        node = node->next;
        free(temp);
    }
    if (prev == NULL) {
        table->ks[index] = ptr->next;
    } else {
        prev->next = ptr->next;
    }
    free(ptr);
    return SUCCESS;
}

int seeTable(Table* table) {
    if(table->csize == 0) {
        printf("Table is empty\n");
        return TABLE_IS_EMPTY;
    }
    printf("********TABLE**********\n");
    for(int index = 0; index != table->msize; index++) {
        printf("Keyspace %d:\n", index);
        KeySpace* ptr = table->ks[index];
        while(ptr != NULL) {
            printf("    Key->%d\n", ptr->key);
            while(ptr->node != NULL) {
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

int individualDelete(Table* table) {
    if (table == NULL || table->csize == 0 || table->msize == 0 || table->ks == NULL) return TABLE_IS_EMPTY;
    for (int index = 0; index < table->msize; index++) {
        KeySpace *ptr = table->ks[index];
        while (ptr != NULL) {
            if(ptr->node == NULL || ptr->node->next == NULL) continue;
            Node *ptrNode = ptr->node->next;
            while (ptrNode != NULL) {
                Node *next_node = ptrNode->next;
                free(ptrNode);
                ptrNode = next_node;
            }
            ptr->node->next = NULL;
            ptr = ptr->next;
        }
    }
    return SUCCESS;
}

int freeTable(Table *table) {
    if (table == NULL || table->csize == 0 || table->msize == 0) return TABLE_IS_EMPTY;
    for (int index = 0; index < table->msize; index++) {
        KeySpace *ptr = table->ks[index];
        while (ptr != NULL) {
            KeySpace* next = ptr->next;
            Node *ptrNode = ptr->node;
            while (ptrNode != NULL) {
                Node *next_node = ptrNode->next;
                free(ptrNode);
                ptrNode = next_node;
            }
            free(ptr);
            ptr = next;
        }
    }
    free(table->ks);
    table->ks = NULL;
    table->csize = 0;
    table->msize = 0;
    return SUCCESS;
}

