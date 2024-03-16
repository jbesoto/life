#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ParseLong(size_t* out, const char* arg);
char** CreateCharGrid(size_t rows, size_t cols, char ch);
void FreeGrid(char** grid, size_t rows);

#endif  // UTILS_H_