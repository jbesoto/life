/**
 * life.c
 *
 * Implements the logic for simulating Conway's Game of Life, including
 * functions for setting up the game environment, simulating generations,
 * and printing the state of the game world.
 * 
 * Usage: ./life [options]
 * Options:
 *   -r, --rows NUM           Set the number of rows (default: 10)
 *   -c, --columns NUM        Set the number of columns (default: 10)
 *   -f, --filename FILENAME  Specify the filename to use (default: life.txt)
 *   -n, --generations NUM    Set the number of generations (default: 10)
 *   --debug                  Enable debug mode
 *   -h, --help               Print this message
 *
 * Author: Juan Diego Becerra
 * Date: 2024-03-16
 */

#include "life.h"

int main(int argc, char* argv[]) {
  config_t config;
  if (ConfigureGame(&config, argc, argv) != 0) {
    return 1;
  }

  char** world = CreateWorld((const config_t*)&config);
  if (!world) {
    fprintf(stderr, "Error: Unable to create world, memory allocation failed.\n");
    return 1;
  }

  if (PlayGame(world, (const config_t*)&config) < 0) {
    fprintf(stderr, "Error: Unable to copy world, memory allocation failed.\n");
    FreeGrid(world, config.rows + kPadding + 1);
    return 1;
  }

  FreeGrid(world, config.rows + kPadding + 1);
  return 0;
}

/**
 * @brief Simulates the game for the specified number of generations.
 *
 * Simulates the game evolution over the configured number of generations,
 * updating the world state in each generation. It prints the world state
 * at each generation step.
 *
 * @param world  A pointer to the current world state.
 * @param config A constant pointer to the game configuration.
 * @return Returns 0 on successful completion of the game, -1 if memory
 *         allocation for the world copy fails.
 *
 * @note This function creates a copy of the world for the simulation process,
 *       which is freed before the function returns.
 */
int PlayGame(char** world, const config_t* config) {
  char** world_cpy = CreateCharGrid(config->rows + kPadding + 1,
                                    config->cols + kPadding + 1, kDeadChar);
  if (!world_cpy) {
    return -1;
  }

  for (size_t gen = 0; gen <= config->generations; gen++) {
    PrintWorld((const char**)world, config, gen);
    SimulateGeneration(world, world_cpy, config);
  }

  FreeGrid(world_cpy, config->rows + kPadding + 1);
  return 0;
}

/**
 * @brief Simulates a single generation step in the game.
 *
 * Updates the state of the world for one generation. This function copies the
 * current world state, then uses it as a reference to update the original
 * world based on the cell state computation rules.
 *
 * @param world     A pointer to the current world state.
 * @param world_cpy A pointer to the copy of the world used for reference.
 * @param config    A constant pointer to the game configuration detailing the
 *                  world size.
 *
 * @note This function relies on `ComputeCellState` to determine the next state
 *       of each cell based on its neighbors.
 */
void SimulateGeneration(char** world, char** world_cpy,
                        const config_t* config) {
  // Copy original world for reference
  for (size_t i = kPadding; i <= config->rows; i++) {
    memcpy(world_cpy[i], world[i], sizeof(char) * config->cols + kPadding + 1);
  }

  for (size_t i = kPadding; i <= config->rows; i++) {
    for (size_t j = kPadding; j <= config->cols; j++) {
      world[i][j] = ComputeCellState((const char**)world_cpy, i, j);
    }
  }
}

/**
 * @brief Computes the next state of a cell based on its neighbors.
 *
 * Determines the next state of a cell at a given position (`row`, `col`) in
 * the world, according to the classic rules of the Game of Life: A cell flips
 * from alive to dead if it has fewer than 2 or more than 3 alive neighbors,
 * and a dead cell becomes alive if it has exactly 3 alive neighbors.
 *
 * @param world The current state of the world as a 2D grid of characters.
 * @param row   The row index of the cell.
 * @param col   The column index of the cell.
 *
 * @return The new state of the cell ('alive' or 'dead').
 */
char ComputeCellState(const char** world, size_t row, size_t col) {
  int neighbor_count = 0;

  // Inspect neighboring cells
  for (size_t i = row - 1; i <= row + 1; i++) {
    for (size_t j = col - 1; j <= col + 1; j++) {
      if (i == row && j == col) {
        continue;
      }
      if (IsAlive(world[i][j])) {
        neighbor_count++;
      }
    }
  }

  char new_state = world[row][col];
  if (IsAlive(world[row][col])) {
    if (neighbor_count < 2 || neighbor_count > 3) {
      new_state = kDeadChar;
    }
  } else if (IsDead(world[row][col]) && neighbor_count == 3) {
    new_state = kAliveChar;
  }

  return new_state;
}

/**
 * @brief Creates the initial game world from a file.
 *
 * Reads the world configuration from a file and initializes the game world
 * grid. The grid is extended with padding as specified by the configuration.
 * Cells are set to alive or dead based on the file contents.
 *
 * @param config A constant pointer to the game configuration.
 * 
 * @return Returns a pointer to the created 2D world grid or NULL if memory
 *         allocation fails.
 *
 * @note This function allocates memory for the world grid that must be freed
 *       by calling `FreeGrid`.
 */
char** CreateWorld(const config_t* config) {
  char** world = CreateCharGrid(config->rows + kPadding + 1,
                                config->cols + kPadding + 1, kDeadChar);
  if (!world) {
    return NULL;
  }

  FILE* fd = fopen(config->filename, "r");
  if (!fd) {
    fprintf(stderr, "Error: Failed to open file, '%s'\n", config->filename);
    return NULL;
  }

  char* line = NULL;
  size_t n = 0;
  for (size_t i = kPadding; i <= config->rows; i++) {
    if (getline(&line, &n, fd) < 0) {
      break;
    }
    for (size_t j = kPadding; j <= config->cols; j++) {
      char linechar = line[j - kPadding];
      if (linechar != kAliveChar) {
        continue;
      }
      world[i][j] = linechar;
    }
  }

  free(line);
  fclose(fd);
  return world;
}

/**
 * @brief Configures the game's settings.
 *
 * Initializes the game configuration based on command-line arguments. If
 * arguments are provided, they override the default settings. Validates
 * input for number of rows, columns, filename for the world configuration,
 * and number of generations.
 *
 * @param config Pointer to the game configuration structure to be initialized.
 * @param argc   The number of command-line arguments.
 * @param args   The command-line arguments provided to the program.
 *
 * @return Returns 0 if the configuration is successful, -1 if there is an
 *         error in the input arguments or their parsing.
 */
int ConfigureGame(config_t* config, int argc, char* args[]) {
  // Set default settings
  config->rows = kDefaults.rows;
  config->cols = kDefaults.cols;
  config->filename = kDefaults.filename;
  config->generations = kDefaults.generations;

  static struct option longopts[] = {
    {"rows",        required_argument, 0,           'r'},
    {"columns",     required_argument, 0,           'c'},
    {"filename",    required_argument, 0,           'f'},
    {"generations", required_argument, 0,           'n'},
    {"debug",       no_argument,       &debug_flag,  1 },
    {"help",        no_argument,       0,           'h'},
    {0, 0, 0, 0}
  };

  int opt;
  char* optstring = "dhr:c:f:n:";
  while ((opt = getopt_long(argc, args, optstring, longopts, 0)) != -1) {
    switch (opt) {
      case 'd':
        debug_flag = 1;
        break;

      case 'r':
        if (ParseLong(&config->rows, optarg) < 0) {
          fprintf(stderr, "Error: Invalid input for rows, '%s'\n", optarg);
          return -1;
        }
        break;

      case 'c':
        if (ParseLong(&config->cols, optarg) < 0) {
          fprintf(stderr, "Error: Invalid input for columns, '%s'\n", optarg);
          return -1;
        }
        break;

      case 'f':
        config->filename = optarg;
        break;

      case 'n':
        if (ParseLong(&config->generations, optarg) < 0) {
          fprintf(stderr, "Error: Invalid input for generations, '%s'\n", optarg);
          return -1;
        }
        break;

      case 'h':
      case '?': 
        PrintUsage();
        return -1;
    }
  }

  return 0;
}

/**
 * @brief Prints the current state of the world to standard output.
 *
 * @param world  The current state of the world as a 2D grid of characters.
 * @param config The game configuration containing the size of the world.
 * @param gen    The current generation number being printed.
 */
void PrintWorld(const char** world, const config_t* config, int gen) {
  if (!debug_flag) { system(CLEAR); }

  printf("Generation %d:\n", gen);
  for (size_t i = kPadding; i <= config->rows; i++) {
    for (size_t j = kPadding; j <= config->cols; j++) {
      printf("%c", world[i][j]);
      if (j == config->cols) {
        putchar('\n');
      }
    }
  }
  puts("================================");
  
  if (!debug_flag) {
    fflush(stdout);
    usleep(kInterval);
  }
}

/**
 * @brief Prints the usage of the program.
*/
void PrintUsage() {
  fprintf(stderr, "Usage: ./life [options]\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -r, --rows NUM           Set the number of rows (default: %ld)\n", kDefaults.rows);
  fprintf(stderr, "  -c, --columns NUM        Set the number of columns (default: %ld)\n", kDefaults.cols);
  fprintf(stderr, "  -f, --filename FILENAME  Specify the filename to use (default: %s)\n", kDefaults.filename);
  fprintf(stderr, "  -n, --generations NUM    Set the number of generations (default: %ld)\n", kDefaults.generations);
  fprintf(stderr, "  --debug                  Enable debug mode\n");
  fprintf(stderr, "  -h, --help               Print this message\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Example:\n");
  fprintf(stderr, "  ./life --rows 20 --columns 20 --filename \"world.txt\" -n 100\n");
}

/**
 * @brief Checks if a cell is alive.
 *
 * @param cell The character representing the cell state.
 *
 * @return 1 if the cell is alive, 0 otherwise.
 */
inline int IsAlive(char cell) { return cell == kAliveChar; }

/**
 * @brief Checks if a cell is dead.
 *
 * @param cell The character representing the cell state.
 *
 * @return 1 if the cell is dead, 0 otherwise.
 */
inline int IsDead(char cell) { return cell == kDeadChar; }