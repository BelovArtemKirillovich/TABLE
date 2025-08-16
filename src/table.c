#include <stdio.h>
#include <stdlib.h>
#include "include/table.h"
#include "include/return_code.h"


int createTable(IndexType msize, Table** __out) {
    if (msize == 0) return INVALID_ARGUMENT_BY_INDEX(0);
    if (__out == NULL) return INVALID_ARGUMENT_BY_INDEX(1);

    Table* table = calloc(1, sizeof(Table));
    if(table == NULL) return ERROR_OF_MEMORY;
    table->ks = calloc(msize, sizeof(KeySpace));
    if(table->ks == NULL) {
        free(table);
        return ERROR_OF_MEMORY;
    }
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
    while(ptr != NULL) {
        if(ptr->key == key) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}



int findAllVersions(Table* table, Node** __out_array, KeyType key, IndexType *n) {
    if (table == NULL) return INVALID_ARGUMENT;
    KeySpace* ptr = find(table, key);
    if (ptr == NULL || ptr->node == NULL) return ELEMENT_NOT_FOUND;
    Node* head = ptr->node;
    IndexType size = 0;
    Node* tmp = head;
    while (tmp != NULL) {
        size++;
        tmp = tmp->next;
    }
    Node* array = calloc(size, sizeof(InfoType));
    if (array == NULL) return ERROR_OF_MEMORY;
    tmp = head;
    for (IndexType i = 0; i < size; i++) {
        array[i].info = tmp->info;
        array[i].release = tmp->release;
        array[i].next = NULL;
        tmp = tmp->next;
    }
    *__out_array = array;
    *n = size;
    return SUCCESS;
}

int createNode(Node** out, RelType release, InfoType info, Node* next) {
    Node* newnode = calloc(1, sizeof(Node));
    if(newnode == NULL) return ERROR_OF_MEMORY;
    newnode->release = release;
    newnode->info = info;
    newnode->next = next;
    *out = newnode;
    return SUCCESS;
}

int insertNode(KeySpace *ks, RelType release, InfoType info) {
    if (ks == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (ks->node == NULL) {
        return createNode(
            &ks->node, 
            release,
            info, 
            NULL
        );
    }
    Node *current = ks->node;
    Node *parent = NULL;
    while (current != NULL) {
        if (current->release == release) return RELEASE_IS_EXISTS;
        if (current->release < release) break;
        parent = current;
        current = current->next;
    }
    if (current == NULL) {
        return createNode(
            &parent->next, 
            release,
            info, 
            NULL
        );
    }
    return createNode(
        (parent == NULL) ? &ks->node : &parent->next, 
        release,
        info, 
        current
    );
}

int __insert(Table* table, KeyType key, RelType* release, InfoType info) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (table->msize == 0) return TABLE_IS_EMPTY;
    if (table->ks == NULL) return TABLE_KEY_SPACE_NOT_INITIALIZE;
    int index = hash(key, table->msize);
    KeySpace* ptr = table->ks[index];
    KeySpace* prev = NULL;
    while(ptr != NULL) {
        if(ptr->key == key) {
            if (release == NULL) {
                return createNode(
                    &ptr->node, 
                    ((ptr->node == NULL) ? 0 : ptr->node->release) + 1,
                    info, 
                    ptr->node
                );
            }
            return insertNode(ptr, *release, info);;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    KeySpace* new = calloc(1, sizeof(KeySpace));
    if(new == NULL) return ERROR_OF_MEMORY;
    new->key = key;
    int code = createNode(
        &new->node, 
        (release == NULL) ? 1 : *release,
        info, 
        NULL
    );
    if (code == ERROR_OF_MEMORY) {
        free(new);
        return ERROR_OF_MEMORY;
    }
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

int insert(Table* table, KeyType key, InfoType info) {
    return __insert(table, key, NULL, info);
}

int insertByRelease(Table* table, KeyType key, RelType release, InfoType info) {
    return __insert(table, key, &release, info);
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
    if (tmp->next != NULL) {
        ptr->node = tmp->next;
        free(tmp);
    } 
    else {
        free(tmp);
        if (prev == NULL) {
            table->ks[index] = ptr->next;
        } else {
            prev->next = ptr->next;
        }
        free(ptr);
        table->csize--;
    }
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

int individualDelete(Table* table) {
    if (table == NULL || table->csize == 0 || table->msize == 0 || table->ks == NULL) return TABLE_IS_EMPTY;
    for (IndexType index = 0; index < table->msize; index++) {
        KeySpace *ptr = table->ks[index];
        while (ptr != NULL) {
            if(ptr->node == NULL || ptr->node->next == NULL) {
                ptr = ptr->next;
                continue;
            }
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

int clearTable(Table *table) {
    if (table == NULL) return INVALID_ARGUMENT;
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
            table->ks[index] = NULL;
        }
    }
    table->csize = 0;
    return SUCCESS;
}

int freeTable(Table** table) {
    if (table == NULL) return INVALID_ARGUMENT;
    clearTable(*table);
    if ((*table) == NULL) return INVALID_ARGUMENT;
    if ((*table)->ks != NULL) {
        free((*table)->ks);
        (*table)->ks = NULL;
    }
    free(*table);
    *table = NULL;
    return SUCCESS;
}

