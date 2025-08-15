#include <stdio.h>
#include <stdlib.h>
#include "include/table.h"
#include "include/return_code.h"


int create(IndexType msize, Table** __out){
    if (__out == NULL) return INVALID_ARGUMENT;

    Table* table = calloc(1, sizeof(Table));
    if(table == NULL) return ERROR_OF_MEMORY;
    /* TODO: check on msize equals 0 */
    table->ks = calloc(msize, sizeof(KeySpace));
    if(table->ks == NULL) return ERROR_OF_MEMORY;
    table->msize = msize;
    table->csize = 0;

    *__out = table;
    return SUCCESS;
}

int hash(KeyType key, int msize) {
    return key % msize;
}

Node* findRelease(Table* table, KeyType key, RelType release) {
    if (table == NULL || table->ks == NULL) return NULL;
    KeySpace* ptr = find(table, key);
    if (ptr == NULL) return NULL;

    Node* ptr_node = ptr->node;
    while (ptr_node != NULL) {
        if(ptr_node->release == release) return ptr_node;
        ptr_node = ptr_node->next;
    }
    return NULL;
}

KeySpace* find(Table* table, KeyType key) {
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

int findAllVersions(Table* table, InfoType** __out_array, KeyType key) {
    if (table == NULL) return INVALID_ARGUMENT;
    if (__out_array == NULL) return INVALID_ARGUMENT;
    KeySpace* ptr = find(table, key);
    if(ptr == NULL) return ELEMENT_NOT_FOUND;
    Node* head = ptr->node;
    if (head == NULL) return ELEMENT_NOT_FOUND;
    InfoType* array = calloc(head->release + 1U, sizeof(InfoType));
    if (array == NULL) return ERROR_OF_MEMORY;

    while (head != NULL) {
        array[head->release] = head->info;
        head = head->next;
    }
    *__out_array = array;
    return SUCCESS;
}


int insert(Table* table, KeyType key, InfoType info) {
    if(table->msize == 0 || table == NULL) return TABLE_IS_EMPTY;
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    KeySpace* prev = NULL;
    while(ptr != NULL) {
        if(ptr->key == key) {
            Node* newnode = malloc(sizeof(Node));
            if(newnode == NULL) return ERROR_OF_MEMORY;
            newnode->release = ptr->node->release + 1;
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
    new->node = malloc(sizeof(Node));
    if(new->node == NULL) {
        free(new);
        return ERROR_OF_MEMORY;
    }
    new->key = key;
    new->node->info = info;
    new->node->release = 1;
    if(prev == NULL) {
        new->next = NULL;
    }
    else {
        new->next = table->ks[index];
    }
    table->ks[index] = new;
    ++table->csize;
    return SUCCESS;
}


int deleteByRelease(Table* table, KeyType key, RelType release) {
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
    table->csize--;
    return SUCCESS;
}


int deleteHeadRelease(Table* table, KeyType key) {
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
    table->csize--;
    return SUCCESS;
}

int deleteKeySpace(Table* table, KeyType key) {
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
    table->csize--;
    return SUCCESS;
}

int seeTable(Table* table) {
    if(table->csize == 0) {
        printf("Table is empty\n");
        return TABLE_IS_EMPTY;
    }
    printf("********TABLE**********\n");
    for(IndexType index = 0; index != table->msize; index++) {
        printf("Keyspace %lu:\n", index);
        KeySpace* ptr = table->ks[index];
        while(ptr != NULL) {
            printf("\tKey->%d\n", ptr->key);
            Node* node = ptr->node;
            while(node != NULL) {
                printf("\t\tRelease->%lu   Info->%u\n", node->release, node->info);
                node = node->next;
            }
            printf("\t\t%p\n", (void *) node);
            ptr = ptr->next;
        }
    }
    return 0;
}

int readIndexTypeInBytes(size_t *number, FILE* file) {
    if (number == NULL) return INVALID_ARGUMENT;
    if (file == NULL) return INVALID_ARGUMENT;
    size_t n = 0;
    for (size_t byte_index = 0; byte_index < sizeof(size_t); byte_index++) {
        int code = fgetc(file);
        /* TODO: check code on correct result */
        n = (code << (8 * byte_index)) + n;
    }
    *number = n;
    return SUCCESS;
}

int import(Table* table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT;
    if (filename == NULL) return INVALID_ARGUMENT;
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    readIndexTypeInBytes(&table->msize, file);
    printf("Msize: %lu\n", table->msize);
    fclose(file);
    return SUCCESS;
}

/*
msize
    len(keysspaces)
        key1INindex1 len(key1INindex1)
            info1
        
    
*/

int putIndexTypeInBytes(size_t number, FILE* file) {
    if (file == NULL) return INVALID_ARGUMENT;
    for (size_t byte_index = 0; byte_index < sizeof(size_t); byte_index++) {
        fputc(number, file);
        number = number >> 8;
    }
    return SUCCESS;
}

int export(Table* table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT;
    if (filename == NULL) return INVALID_ARGUMENT;
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    putIndexTypeInBytes(table->msize, file);
    fclose(file);
    return SUCCESS;
}

int individualDelete(Table* table) {
    if (table == NULL || table->csize == 0 || table->msize == 0 || table->ks == NULL) return TABLE_IS_EMPTY;
    for (IndexType index = 0; index < table->msize; index++) {
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
    if (table == NULL) return TABLE_IS_EMPTY;
    if (table->ks != NULL) {
        for (IndexType index = 0; index < table->msize; index++) {
            KeySpace *ptr = table->ks[index];
            while (ptr != NULL) {
                KeySpace* next = ptr->next;
                Node *ptrNode = ptr->node;
                while (ptrNode != NULL) {
                    Node *next_node = ptrNode->next;
                    free(ptrNode);
                    ptrNode = next_node;
                    table->csize--;
                }
                free(ptr);
                ptr = next;
            }
        }
        free(table->ks);
    }
    table->ks = NULL;
    table->csize = 0;
    table->msize = 0;
    free(table);
    return SUCCESS;
}

