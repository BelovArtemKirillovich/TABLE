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
    printf("5. See table\n");
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
    int command;
    Table* table = NULL;
    do {
        int tmp, key, release, info;
        menu();
        if(checkCom(&command) == -1) {
            return close(table, SUCCESS);
        }
        switch(command)  {
            case 0:
                return close(table, SUCCESS);
            case 1:
                freeTable(& table);
                IndexType msize;
                int code;
                do {
                    printf("msize:\n");
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
                    fprintf(stderr, "Error: Not fount memory to initialize table.");
                    return close(table, code);
                }  
                break;
            case 2:
                printf("key: \n");
                checkInt(&key);
                printf("Info: \n");
                checkInt(&info);
                tmp = insert(table, key, info);
                if(tmp == 1) {
                    printf("ERROR\nTable not found\n");
                }
                else if(tmp == 2) {
                    printf("ERROR\nError of memory\nNew element didn`t create\n");
                }
                printTable(table);
                break;
            case 3:
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
                printTable(table);
                break;
            case 4:
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
                printTable(table);
                break;
            case 5:
                printTable(table);   
                break;
            case 6:
                importTable(table, "tests/table.bin");      
                break;
            case 7:
                exportTable(table, "tests/table.bin");
                break;
            case 8:
                tmp = individualDelete(table);
                if(tmp == TABLE_IS_EMPTY) {
                    printf("ERROR\nTable is empty\n");
                }
                printTable(table);
                break;
            case 9:
                clearTable(table);
                break;
        }
    }while(command != 0);
    return 0;
}