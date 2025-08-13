#include <stdio.h>
#include <limits.h>


int inputInt(int *x, int min, int max) {
    int count = scanf("%d", x);
    while (count != 1 || *x < min || *x > max) {
        printf("Error. Please correct input:\n");
        scanf("%*[^\n]");
        count = scanf("%d", x);
        if (count == EOF) {
            return -1;
        }
    }
    return 0;
}

int checkCom(int *x) {
    return inputInt(x, 0, 9);
}

int checkInt(int *x) {
    return inputInt(x, INT_MIN, INT_MAX);
}