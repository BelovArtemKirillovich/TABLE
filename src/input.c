#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

int inputUInt64(uint64_t *out, uint64_t min, uint64_t max) {
    if (out == NULL) return -2;
    int count = scanf("%lu", out);
    while (count != 1 || *out < min || *out > max) {
        printf("Error. Please correct input:\n");
        scanf("%*[^\n]");
        count = scanf("%lu", out);
        if (count == EOF) return -1;
    }
    return 0;
}

int inputInt64(int64_t *out, int64_t min, int64_t max) {
    if (out == NULL) return -2;
    int count = scanf("%ld", out);
    while (count != 1 || *out < min || *out > max) {
        printf("Error. Please correct input:\n");
        scanf("%*[^\n]");
        count = scanf("%ld", out);
        if (count == EOF) return -1;
    }
    return 0;
}

int inputInt32(int32_t *out, int32_t min, int32_t max) {
    int64_t number = 0;
    int code = inputInt64(&number, min, max);
    if (code == 0) *out = (int32_t) number;
    return code;
}

int inputUInt32(uint32_t *out, uint32_t min, uint32_t max) {
    uint64_t number = 0;
    int code = inputUInt64(&number, min, max);
    if (code == 0) *out = (uint32_t) number;
    return code;
}

int inputInt16(int16_t *out, int16_t min, int16_t max) {
    int64_t number = 0;
    int code = inputInt64(&number, min, max);
    if (code == 0) *out = (int16_t) number;
    return code;
}

int inputUInt16(uint16_t *out, uint16_t min, uint16_t max) {
    uint64_t number = 0;
    int code = inputUInt64(&number, min, max);
    if (code == 0) *out = (uint16_t) number;
    return code;
}

int inputInt8(int8_t *out, int8_t min, int8_t max) {
    int64_t number = 0;
    int code = inputInt64(&number, min, max);
    if (code == 0) *out = (int8_t) number;
    return code;
}

int inputUInt8(uint8_t *out, uint8_t min, uint8_t max) {
    uint64_t number = 0;
    int code = inputUInt64(&number, min, max);
    if (code == 0) *out = (uint8_t) number;
    return code;
}


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