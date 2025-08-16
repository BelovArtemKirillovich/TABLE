#ifndef INPUT_H
#define INPUT_H
#include <stdint.h>

int inputUInt64(uint64_t *out, uint64_t min, uint64_t max);
int inputInt64(int64_t *out, int64_t min, int64_t max);
int inputInt32(int32_t *out, int32_t min, int32_t max);
int inputUInt32(uint32_t *out, uint32_t min, uint32_t max);
int inputInt16(int16_t *out, int16_t min, int16_t max);
int inputUInt16(uint16_t *out, uint16_t min, uint16_t max);
int inputInt8(int8_t *out, int8_t min, int8_t max);
int inputUInt8(uint8_t *out, uint8_t min, uint8_t max);
int freadline(FILE* stream, char** out);
int readline(char** out);

int inputInt(int *x, int min, int max);
int checkCom(int *k);
int checkInt(int *k);
int checkUInt(uint32_t *x);

#endif