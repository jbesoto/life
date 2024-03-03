#ifndef LIFE_H_
#define LIFE_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
    size_t rows;
    size_t cols;
    char* filename;
    size_t generations;
} config_t;

typedef struct Coordinate {
    size_t x;
    size_t y;
} Coordinate;

const config_t kDefaults = {10, 10, "life.txt", 10};
const size_t kPadding = 1;

int IsAlive(char cell);
int IsDead(char cell);
int ParseLong(const char* arg, size_t* out);
int ConfigureGame(config_t* config, int argc, char* args[]);
int FreeGrid(char** grid, size_t rows);
char** CreateCharGrid(size_t rows, size_t cols, char ch);
char** CreateWorldFromFile(FILE* fd, const config_t* config);
char ComputeNewState(const char** world, const Coordinate *coord);
void play(char** world, const config_t* config);
void PrintWorld(const char** world, config_t* config, int gen);

#endif  // LIFE_H_
