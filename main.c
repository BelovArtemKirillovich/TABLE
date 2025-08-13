#include <stdio.h>
#include "table.h"

void menu() {
    printf("********MENU*********\n");
    printf("0. Exit\n");
    printf("1. Create table\n");
    printf("2. Insert element\n");
    printf("3. Delete element\n");
    printf("4. Find by key\n");
    printf("5. See table\n");
    printf("6. Import table\n");
    printf("7. Export table\n");
    printf("8. Find all versions by key\n");
    printf("9. Reorganizing table\n");
}



int main() {
    int command, key, release, info, msize, tmp;
    KeySpace* ks;
    Table* table = create(0);
    do {
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
                if(table->ks != NULL){
                    freeTable(table);
                }
                printf("msize:\n");
                checkInt(&msize);
                table = create(msize);
                if(table == NULL) {
                    printf("ERROR\nError of memory\nTable didn`t create\n");
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
                break;
            case 3:
                            
                break;
            case 4:
                            
                break;
            case 5:
                seeTable(table);   
                break;
            case 6:
                            
                break;
            case 7:
                            
                break;
            case 8:
                            
                break;
            case 9:
                            
                break;
        }
    }while(command != 0);
    return 0;
}