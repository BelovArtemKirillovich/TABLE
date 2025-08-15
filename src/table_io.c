#include "include/table_io.h"
#include "include/return_code.h"

int getIndexTypeInBytes(IndexType *number, FILE* file) {
    if (number == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    IndexType n = 0;
    IndexType byte_index = 0;
    while (!feof(file) && !ferror(file) && byte_index < sizeof(IndexType)) {
        int code = fgetc(file);
        n = (code << (8 * byte_index)) + n;
        byte_index++;
    }
    if (feof(file) && byte_index < sizeof(IndexType)) {
        return FILE_IS_END;
    }
    if (ferror(file)) {
        return ERROR_IN_FILE;
    }
    *number = n;
    return SUCCESS;
}

int getKeyTypeInBytes(KeyType *number, FILE* file) {
    if (number == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    KeyType n = 0;
    KeyType byte_index = 0;
    while (!feof(file) && !ferror(file) && byte_index < sizeof(KeyType)) {
        int code = fgetc(file);
        n = (code << (8 * byte_index)) + n;
        byte_index++;
    }
    if (feof(file) && byte_index < sizeof(KeyType)) {
        return FILE_IS_END;
    }
    if (ferror(file)) {
        return ERROR_IN_FILE;
    }
    *number = n;
    return SUCCESS;
}

int getInfoTypeInBytes(InfoType *number, FILE* file) {
    if (number == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    InfoType n = 0;
    InfoType byte_index = 0;
    while (!feof(file) && !ferror(file) && byte_index < sizeof(InfoType)) {
        int code = fgetc(file);
        n = (code << (8 * byte_index)) + n;
        byte_index++;
    }
    if (feof(file) && byte_index < sizeof(InfoType)) {
        return FILE_IS_END;
    }
    if (ferror(file)) {
        return ERROR_IN_FILE;
    }
    *number = n;
    return SUCCESS;
}

int getRelTypeInBytes(RelType *number, FILE* file) {
    if (number == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    RelType n = 0;
    RelType byte_index = 0;
    while (!feof(file) && !ferror(file) && byte_index < sizeof(RelType)) {
        int code = fgetc(file);
        n = (code << (8 * byte_index)) + n;
        byte_index++;
    }
    if (feof(file) && byte_index < sizeof(RelType)) {
        return FILE_IS_END;
    }
    if (ferror(file)) {
        return ERROR_IN_FILE;
    }
    *number = n;
    return SUCCESS;
}

int importTable(Table** table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (filename == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    IndexType msize;
    int code = getIndexTypeInBytes(&msize, file);
    // code == INVALID_ARGUMENT_BY_INDEX(0) is unavailable
    // code == INVALID_ARGUMENT_BY_INDEX(1) is unavailable, and at each others
    if (code == FILE_IS_END || code == ERROR_IN_FILE) {
        fclose(file);
        return code;
    }

    if (*table != NULL) {
        freeTable(table);
    }
    code = createTable(msize, table);
    // code == INVALID_ARGUMENT_BY_INDEX(1) is unavailable
    if (code == INVALID_ARGUMENT_BY_INDEX(0)) {
        return TABLE_IS_EMPTY;
    }

    /* import table content kay-space array */
    Table *t = *table;
    for(IndexType index = 0; index < t->msize; index++) {
        IndexType ks_list_length;
        code = getIndexTypeInBytes(&ks_list_length, file);
        if (code == FILE_IS_END || code == ERROR_IN_FILE) {
            fclose(file);
            return code;
        }
        for (IndexType subindex = 0; subindex < ks_list_length; subindex++) {
            KeyType key;
            code = getKeyTypeInBytes(&key, file);
            if (code == FILE_IS_END || code == ERROR_IN_FILE) {
                fclose(file);
                return code;
            }
            IndexType nodes_list_length;
            code = getIndexTypeInBytes(&nodes_list_length, file);
            if (code == FILE_IS_END || code == ERROR_IN_FILE) {
                fclose(file);
                return code;
            }
            for (IndexType node_index = 0; node_index < nodes_list_length; node_index++) {
                RelType release;
                code = getRelTypeInBytes(&release, file);
                if (code == FILE_IS_END || code == ERROR_IN_FILE) {
                    fclose(file);
                    return code;
                }
                InfoType info;
                code = getIndexTypeInBytes(&info, file);
                if (code == FILE_IS_END || code == ERROR_IN_FILE) {
                    fclose(file);
                    return code;
                }
                insertByRelease(t, key, release, info);
                // TODO: need insert(table, key, release, info);
            }
        }
    }
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

int putKeyTypeInBytes(KeyType key, FILE* file) {
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    for (KeyType byte_index = 0; byte_index < sizeof(KeyType); byte_index++) {
        fputc(key, file);
        if (ferror(file)) {
            return ERROR_IN_FILE;
        }
        key = key >> 8;
    }
    return SUCCESS;
}

int putRelTypeInBytes(RelType key, FILE* file) {
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    for (RelType byte_index = 0; byte_index < sizeof(RelType); byte_index++) {
        fputc(key, file);
        if (ferror(file)) {
            return ERROR_IN_FILE;
        }
        key = key >> 8;
    }
    return SUCCESS;
}

int putInfoTypeInBytes(InfoType key, FILE* file) {
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    for (InfoType byte_index = 0; byte_index < sizeof(InfoType); byte_index++) {
        fputc(key, file);
        if (ferror(file)) {
            return ERROR_IN_FILE;
        }
        key = key >> 8;
    }
    return SUCCESS;
}

IndexType ksListLength(KeySpace *ks) {
    IndexType length = 0;
    while (ks != NULL) {
        length++;
        ks = ks->next;
    }
    return length;
}

IndexType nodeListLength(Node *node) {
    IndexType length = 0;
    while (node != NULL) {
        length++;
        node = node->next;
    }
    return length;
}

int exportKeySpace(KeySpace* ks, FILE* file) {
    if (file == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    while(ks != NULL) {
        int code = putKeyTypeInBytes(ks->key, file);
        if (code == ERROR_IN_FILE) {
            return ERROR_IN_FILE;
        }
        Node* node = ks->node;
        code = putIndexTypeInBytes(nodeListLength(node), file);
        if (code == ERROR_IN_FILE) {
            return ERROR_IN_FILE;
        }
        
        while (node != NULL) {
            code = putRelTypeInBytes(node->release, file);
            if (code == ERROR_IN_FILE) {
                return ERROR_IN_FILE;
            }
            code = putKeyTypeInBytes(node->info, file);
            if (code == ERROR_IN_FILE) {
                return ERROR_IN_FILE;
            }
            node = node->next;
        }
        ks = ks->next;
    }
    return SUCCESS;
}

int exportTable(Table* table, const char* filename) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (filename == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    FILE* file = fopen(filename, "wb");
    if (file == NULL) return FILE_CAN_NOT_OPEN;
    
    /* Export max-size of table */
    int code = putIndexTypeInBytes(table->msize, file);
    // code == INVALID_ARGUMENT_BY_INDEX(1) is unavailable, and in other calls
    if (code == ERROR_IN_FILE) {
        fclose(file);
        remove(filename);
        return ERROR_IN_FILE;
    }

    /* Export table kay-space array */
    for(IndexType index = 0; index < table->msize; index++) {
        KeySpace* ks = table->ks[index];
        code = putIndexTypeInBytes(ksListLength(ks), file);
        if (code == ERROR_IN_FILE) {
            fclose(file);
            remove(filename);
            return ERROR_IN_FILE;
        }
        code = exportKeySpace(ks, file);
        if (code == ERROR_IN_FILE) {
            fclose(file);
            remove(filename);
            return ERROR_IN_FILE;
        }
    }

    fclose(file);
    return SUCCESS;
}

int fprintTable(Table* table, FILE* output) {
    if (table == NULL) return INVALID_ARGUMENT_BY_INDEX(0);
    if (output == NULL) return INVALID_ARGUMENT_BY_INDEX(1);
    if (table->ks == NULL) return TABLE_KEY_SPACE_NOT_INITIALIZE;
    fprintf(output, "Table(max-size=%u, capacity=%u) {\n", table->msize, table->csize);
    for(IndexType index = 0; index < table->msize; index++) {
        KeySpace* ptr = table->ks[index];
        if (ptr == NULL) continue;
        fprintf(output, "\tKeyspace %u:\n", index);
        while(ptr != NULL) {
            fprintf(output, "\t\tKey->%d\n", ptr->key);
            Node* node = ptr->node;
            while (node != NULL) {
                fprintf(output, "\t\t\tRelease->%u   Info->%u\n", node->release, node->info);
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
