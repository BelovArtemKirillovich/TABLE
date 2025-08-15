#ifndef TABLE_IO_H
#define TABLE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include "table.h"

int exportTable(Table* table, const char* filename);
int importTable(Table** table, const char* filename);
int fprintTable(Table* table, FILE* output);
int printTable(Table* table);

#endif // TABLE_IO_H