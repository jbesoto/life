#ifndef LIFE_H_
#define LIFE_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

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

static int debug_flag = 0;
static const config_t kDefaults = {10, 10, "life.txt", 10};
static const size_t kPadding = 1;
static const useconds_t kInterval = 0;  // microseconds
static const char kAliveChar = '*';
static const char kDeadChar = ' ';

int ConfigureGame(config_t* config, int argc, char* args[]);
char ComputeCellState(const char** world, size_t row, size_t col);
char** CreateWorld(FILE* fd, const config_t* config);
static inline int IsAlive(char cell);
static inline int IsDead(char cell);
int PlayGame(char** world, const config_t* config);
static inline void PrintUsage(void);
void PrintWorld(const char** world, const config_t* config, int gen);
void SimulateGeneration(char** world, char** world_ref, const config_t* config);

#endif  // LIFE_H_
