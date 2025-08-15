#include <stdio.h>
#include "include/input.h"
#include "include/table.h"
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
}

void menuDelete() {
    printf("********DELETE*********\n");
    printf("1. Delete element by key and release\n");
    printf("2. Delete element by last release\n");
    printf("3. Delete element by key (all release)\n");
}

void menuFind() {
    printf("**********FIND***********\n");
    printf("1. Find by key and release\n");
    printf("2. Find all release by key\n");
}



int main() {
    int command;
    Table* table = NULL;
    do {
        int tmp, key, release, info, msize;
        menu();
        if(checkCom(&command) == -1) {
            freeTable(table);
            free(table);
            printf("Programme closed.\n");
            return 0;
        }
        switch(command)  {
            case 0:
                freeTable(table);
                free(table);
                printf("Programme closed.\n");
                return 0;
            case 1:
                freeTable(table);
                printf("msize:\n");
                checkInt(&msize);
                int code = create(msize, &table);
                if (code == INVALID_ARGUMENT) {

                } else if (code == ERROR_OF_MEMORY) {
                    printf("ERROR\nError of memory\n");
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
                seeTable(table);
                break;
            case 3:
                menuDelete();
                inputInt(&command, 1, 3);
                printf("Key\n");
                checkInt(&key);
                if(command == 1) {
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
                    tmp = deleteHeadRelease(table, key);
                    if(tmp == TABLE_IS_EMPTY) {
                        printf("ERROR\nTable is empty\n");
                    }
                    else if(tmp == ELEMENT_NOT_FOUND) {
                        printf("ERROR\nElement not found\n");
                    }
                }
                else if(command == 3) {
                    tmp = deleteKeySpace(table, key);
                    if(tmp == TABLE_IS_EMPTY) {
                        printf("ERROR\nTable is empty\n");
                    }
                    else if(tmp == ELEMENT_NOT_FOUND) {
                        printf("ERROR\nElement not found\n");
                    }
                }
                seeTable(table);
                break;
            case 4:
                menuFind();
                inputInt(&command, 1, 2);
                printf("Key:\n");
                checkInt(&key);
                if(command == 1) {
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
                    InfoType* array = NULL;
                    tmp = findAllVersions(table, &array, key);
                    if(tmp == ELEMENT_NOT_FOUND) {
                        printf("ERROR\nElement not found\n");
                    }
                    else if (tmp == SUCCESS) {
                        //печать элементов
                        free(array);
                    }
                }
                seeTable(table);
                break;
            case 5:
                seeTable(table);   
                break;
            case 6:
                            
                break;
            case 7:
                            
                break;
            case 8:
                tmp = individualDelete(table);
                if(tmp == TABLE_IS_EMPTY) {
                    printf("ERROR\nTable is empty\n");
                }
                seeTable(table);
                break;
        }
    }while(command != 0);
    return 0;
}