#ifndef LIFE_H_
#define LIFE_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Struct for storing game configurations
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

static const config_t kDefaults = {10, 10, "life.txt", 10};
static const size_t kPadding = 1;
static const useconds_t kInterval = 800000;  // microseconds
static const char kAliveChar = '*';
static const char kDeadChar = ' ';

static inline size_t min(size_t a, size_t b);
int IsAlive(char cell);
int IsDead(char cell);
int ParseLong(size_t* out, const char* arg);
int ConfigureGame(config_t* config, int argc, char* args[]);
int FreeGrid(char** grid, size_t rows);
char** CreateCharGrid(size_t rows, size_t cols, char ch);
char** CreateWorldFromFile(FILE* fd, const config_t* config);
char ComputeNewState(const char** world, const Coordinate* coord);
void play(char** world, const config_t* config);
void PrintWorld(const char** world, config_t* config, int gen);

#endif  // LIFE_H_
