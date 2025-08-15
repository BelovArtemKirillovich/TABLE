#include "include/table_io.h"
#include "include/return_code.h"

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

int importTable(Table* table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT;
    if (filename == NULL) return INVALID_ARGUMENT;
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    readIndexTypeInBytes(&table->msize, file);
    printf("Msize: %lu\n", table->msize);
    fclose(file);
    return SUCCESS;
}

int putIndexTypeInBytes(size_t number, FILE* file) {
    if (file == NULL) return INVALID_ARGUMENT;
    for (size_t byte_index = 0; byte_index < sizeof(size_t); byte_index++) {
        fputc(number, file);
        number = number >> 8;
    }
    return SUCCESS;
}

int exportTable(Table* table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT;
    if (filename == NULL) return INVALID_ARGUMENT;
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    putIndexTypeInBytes(table->msize, file);
    fclose(file);
    return SUCCESS;
}

int fprintTable(Table* table, FILE* output) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (output == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
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

int printTable(Table* table) {
    return fprintTable(table, stdout);
}
