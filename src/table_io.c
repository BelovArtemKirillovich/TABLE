#include "include/table_io.h"
#include "include/return_code.h"

int readIndexTypeInBytes(IndexType *number, FILE* file) {
    if (number == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    IndexType n = 0;
    IndexType byte_index = 0;
    while (!feof(file) && !ferror(file) && byte_index < sizeof(IndexType)) {
        int code = fgetc(file);
        n = (code << (8 * byte_index)) + n;
        byte_index++;
    }
    if (feof(file)) {
        return FILE_IS_END;
    }
    if (ferror(file)) {
        return ERROR_IN_FILE;
    }
    *number = n;
    return SUCCESS;
}

int importTable(Table* table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (filename == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    /* TODO: Add importing table */
    fclose(file);
    return SUCCESS;
}

int putIndexTypeInBytes(IndexType number, FILE* file) {
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    for (IndexType byte_index = 0; byte_index < sizeof(IndexType); byte_index++) {
        fputc(number, file);
        if (ferror(file)) {
            return ERROR_IN_FILE;
        }
        number = number >> 8;
    }
    return SUCCESS;
}

int exportTable(Table* table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (filename == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    /* TODO: Add importing table */
    fclose(file);
    return SUCCESS;
}

int fprintTable(Table* table, FILE* output) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (output == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    if (table->ks == NULL) return TABLE_KEY_SPACE_NOT_INITIALIZE;
    fprintf(output, "Table(max-size=%lu, capacity=%lu) {\n", table->msize, table->csize);
    for(IndexType index = 0; index != table->msize; index++) {
        KeySpace* ptr = table->ks[index];
        if (ptr == NULL) continue;
        fprintf(output, "\tKeyspace %lu:\n", index);
        while(ptr != NULL) {
            fprintf(output, "\t\tKey->%d\n", ptr->key);
            Node* node = ptr->node;
            while (node != NULL) {
                fprintf(output, "\t\t\tRelease->%lu   Info->%u\n", node->release, node->info);
                node = node->next;
            }
            ptr = ptr->next;
        }
    }
    fprintf(output, "}\n");
    return 0;
}

int printTable(Table* table) {
    return fprintTable(table, stdout);
}
