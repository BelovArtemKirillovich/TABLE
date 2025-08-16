#include <stdio.h>
#include "include/input.h"
#include "include/table.h"
#include "include/table_io.h"
#include "include/return_code.h"

void menu() {
    printf("********MENU*********\n");
    printf("0. Exit\n");
    printf("1. Create table\n");
    printf("2. Insert element\n");
    printf("3. Delete element\n");
    printf("4. Find element\n");
    printf("5. Print table\n");
    printf("6. Import table\n");
    printf("7. Export table\n");
    printf("8. Reorganizing table\n");
    printf("9. Clear table\n");
}

void menuDelete() {
    printf("********DELETE*********\n");
    printf("0. Back\n");
    printf("1. Delete element by key and release\n");
    printf("2. Delete element by last release\n");
    printf("3. Delete element by key (all release)\n");
}

void menuFind() {
    printf("**********FIND***********\n");
    printf("0. Back\n");
    printf("1. Find by key and release\n");
    printf("2. Find all release by key\n");
}

int close(Table *table, int code) {
    if (table != NULL) freeTable(&table);
    printf("Programme closed.\n");
    return code;
}

int main() {
    uint8_t command;
    Table* table = NULL;
    do {
        menu();
        if(inputUInt8(&command, 0, 9) == -1) {
            return close(table, SUCCESS);
        }
        switch(command)  {
            case 0: {
                return close(table, SUCCESS);
            }
            case 1: {
                freeTable(&table);
                IndexType msize;
                int code;
                do {
                    printf("Enter max-size of table: \n");
                    code = inputUInt32(&msize, 1U, UINT32_MAX);
                    // code == INVALID_ARGUMENT_BY_INDEX(0) is unavailable point
                    if (code == ELEMENT_NOT_FOUND) {
                        fprintf(stderr, "Error: Can't read next number.");
                        return close(table, code);
                    }
                } while (code != SUCCESS);
                code = createTable(msize, &table);
                // code == INVALID_ARGUMENT_BY_INDEX(0) is unavailable point
                // code == INVALID_ARGUMENT_BY_INDEX(1) is unavailable point
                if (code == ERROR_OF_MEMORY) {
                    fprintf(stderr, "Error: Not found memory to initialize table.");
                    return close(table, code);
                }  
                break;
            }
            case 2: {
                KeyType key;
                InfoType info;
                int code;
                do {
                    printf("Enter key: ");
                    code = inputUInt32(&key, 0U, UINT32_MAX);
                    // code == INVALID_ARGUMENT_BY_INDEX(0) is unavailable point
                    if (code == ELEMENT_NOT_FOUND) {
                        fprintf(stderr, "Error: Can't read next number.");
                        return close(table, code);
                    }
                } while (code != SUCCESS);
                do {
                    printf("Enter info: ");
                    code = inputUInt32(&info, 0U, UINT32_MAX);
                    // code == INVALID_ARGUMENT_BY_INDEX(0) is unavailable point
                    if (code == ELEMENT_NOT_FOUND) {
                        fprintf(stderr, "Error: Can't read next number.");
                        return close(table, code);
                    }
                } while (code != SUCCESS);
                code = insert(table, key, info);
                // code == TABLE_IS_EMPTY is unavailable
                // code == RELEASE_IS_EXISTS is unavailable
                // code == TABLE_KEY_SPACE_NOT_INITIALIZE is unavailable
                if (code == INVALID_ARGUMENT_BY_INDEX(0)) {
                    fprintf(stderr, "Error: Table is not initialized, create it, and try again.\n");
                } else if (code == ERROR_OF_MEMORY) {
                    fprintf(stderr, "Error: Not found memory to inserting.");
                    return close(table, code);
                }
                break;
            }
            case 3: {
                int key, release, tmp;
                menuDelete();
                inputInt(&command, 0, 3);
                if(command == 1) {
                    printf("Key\n");
                    checkInt(&key);
                    printf("Release:\n");
                    checkInt(&release);
                    tmp = deleteByRelease(table, key, release);
                    if(tmp == TABLE_IS_EMPTY) {
                        printf("ERROR\nTable is empty\n");
                    }
                    else if(tmp == ELEMENT_NOT_FOUND) {
                        printf("ERROR\nElement not found\n");
                    }
                }
                else if(command == 2) {
                    printf("Key\n");
                    checkInt(&key);
                    tmp = deleteHeadRelease(table, key);
                    if(tmp == TABLE_IS_EMPTY) {
                        printf("ERROR\nTable is empty\n");
                    }
                    else if(tmp == ELEMENT_NOT_FOUND) {
                        printf("ERROR\nElement not found\n");
                    }
                }
                else if(command == 3) {
                    printf("Key\n");
                    checkInt(&key);
                    tmp = deleteKeySpace(table, key);
                    if(tmp == TABLE_IS_EMPTY) {
                        printf("ERROR\nTable is empty\n");
                    }
                    else if(tmp == ELEMENT_NOT_FOUND) {
                        printf("ERROR\nElement not found\n");
                    }
                }
                else if(command == 0) { // ERROR
                    printTable(table);
                    continue;
                }
                break;
            }
            case 4: {
                int key, release, tmp;
                menuFind();
                inputInt(&command, 0, 2);
                if(command == 1) {
                    printf("Key:\n");
                    checkInt(&key);
                    printf("Release:\n");
                    checkInt(&release);
                    Node* node = findRelease(table, key, release);
                    if(node == NULL) {
                        printf("ERROR\nElement not found\n");
                    }
                    else {
                        printf("Key->%d   Info->%d   Release->%d\n", key, node->info, release);
                    }
                }
                else if(command == 2) {
                    printf("Key:\n");
                    checkInt(&key);
                    IndexType n = 0;
                    InfoType* array = NULL;
                    tmp = findAllVersions(table, &array, key, &n);
                    if(tmp == ELEMENT_NOT_FOUND) {
                        printf("ERROR\nElement not found\n");
                    }
                    else if (tmp == SUCCESS) {
                        printf("Key->%d\n", key);
                        for(IndexType i = n; i != 0; i--) {
                            printf("\tRelease->%d   Info->%d\n", (i + 1), array[i]);
                        }
                        free(array);
                    }
                }
                else if(command == 0) { //ERROR
                    printTable(table);
                    continue;
                }
                break;
            }
            case 5: {
                int code = printTable(table); 
                // code == INVALID_ARGUMENT_BY_INDEX(0) is unavailable
                // code == TABLE_KEY_SPACE_NOT_INITIALIZE is unavailable
                if (code == ELEMENT_NOT_FOUND) {
                    fprintf(stderr, "CriticalError: Can't find 'stdin' stream");
                    return close(table, code);
                }
                break;
            }
            case 6: {
                char* filename = "tests/table.bin";
                int code = importTable(&table, filename);
                // code == INVALID_ARGUMENT_BY_INDEX(0) is unavailable
                // code == INVALID_ARGUMENT_BY_INDEX(1) is unavailable, and at each others
                if (code == FILE_IS_END) {
                    fprintf(stderr, "Error: file for importing so small.\n");
                } else if (code == FILE_CAN_NOT_OPEN) {
                    fprintf(stderr, "Error: file '%s' not found.\n", filename);
                } else if (code == TABLE_IS_EMPTY) {
                    fprintf(stderr, "Error: exporting file contains invalid table size: zero.\n");
                } else if (code == ERROR_IN_FILE) {
                    perror("Error: on read file on importing table\n");
                } else if (code == INVALID_FILE_CONTENT) {
                    fprintf(stderr, "Error: File %s content is invalid", filename);
                }
                break;
            }
            case 7: {
                char* filename = "tests/table.bin";
                int code = exportTable(table, filename);
                // code == INVALID_ARGUMENT_BY_INDEX(1) is unavailable, and in other calls
                if (code == INVALID_ARGUMENT_BY_INDEX(0)) {
                    fprintf(stderr, "Error: Table is not initialized, create it, and try again.\n");
                } else if (code == FILE_CAN_NOT_OPEN) {
                    fprintf(stderr, "Error: Can't open file '%s' to export table\n", filename);
                } else if (code == ERROR_IN_FILE) {
                    perror("Error: on save to file on exporting table\n");
                }
                break;
            }
            case 8: {
                int tmp;
                tmp = individualDelete(table);
                if(tmp == TABLE_IS_EMPTY) {
                    printf("ERROR\nTable is empty\n");
                }
                break;
            }
            case 9: {
                clearTable(table);
                break;
            }
        }
    } while (command != 0);
    return 0;
}