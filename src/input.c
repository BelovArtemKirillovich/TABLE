#include <stdio.h>
#include <limits.h>
#include <stdint.h>

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

int inputUInt(uint32_t *x, uint32_t min, uint32_t max) {
    int count = scanf("%u", x);
    while (count != 1 || *x < min || *x > max) {
        printf("Error. Please correct input:\n");
        scanf("%*[^\n]");
        count = scanf("%u", x);
        if (count == EOF) {
            return -1;
        }
    }
    return 0;
}

int checkCom(int *x) {
    return inputInt(x, 0, 8);
}

int checkInt(int *x) {
    return inputInt(x, INT_MIN, INT_MAX);
}

int checkUInt(uint32_t *x) {
    return inputUInt(x, 0, UINT_MAX);
}