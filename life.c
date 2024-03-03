#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
const char* kClearScreenCode = "\033[H\033[J";
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Struct for storing game configurations
typedef struct {
    size_t rows;
    size_t cols;
    char* filename;
    size_t generations;
} config_t;

static const config_t kDefaults = {10, 10, "life.txt", 10};

// Ensures that world traversal is kept within the valid cells
static const size_t kOffset = 1;

static const useconds_t kInterval = 800000;  // microseconds


// Determines if a cell is alive.
//
// Args:
//   cell: A character representing the current state of the cell.
//
// Returns:
//   1 if the cell is alive ('*'), 0 otherwise.
int IsAlive(char cell) {
    return cell == '*';
}

// Determines if a cell is dead.
//
// Args:
//   cell: A character representing the current state of the cell.
//
// Returns:
//   1 if the cell is dead ('-'), 0 otherwise.
int IsDead(char cell) {
    return cell == '-';
}

// Prints the current state of the world for a given generation.
//
// Args:
//   world:  A pointer to a 2D array of characters where each character represents
//           a cell in the world. '*' for a live cell and '-' for a dead cell.
//   config: A pointer to the config_t structure where the settings are stored.
//   gen:    The current generation number that is being printed.
void PrintWorld(const char** world, config_t* config, int gen) {
#ifdef _WIN32
    system("cls");
#else
    printf("%s", kClearScreenCode);
#endif

    printf("Generation %d:\n", gen);
    for (size_t i = 0 + kOffset; i <= config->rows; i++) {
        for (size_t j = 0 + kOffset; j <= config->cols; j++) {
            printf("%c", world[i][j]);
            if (j == config->cols) {
                putchar('\n');
            }
        }
    }
    printf("================================\n");
    fflush(stdout);
    usleep(kInterval);
}

// Parses a string representing a positive integer into a `size_t`.
//
// Args:
//   arg: A pointer to the null-terminated string to be parsed.
//   out: A pointer to the variable where the parsed value will be stored.
//
// Returns:
//   0 if the parsing is successful and the value is a positive integer.
//   1 if the parsing fails or the parsed integer is not positive.
//
// Notes:
//   The function uses `strtol` and checks for errors during the conversion. If
//   `errno` is set or the parsed value is less than 1, it's considered an error.
int ParseLong(const char* arg, size_t* out) {
    char* end;
    long value = strtol(arg, &end, 10);
    
    if (errno != 0 || value < 1) {
       return 1;
    }
    *out = (size_t)value;
    return 0;
}

// Sets the game configurations based on the provided command-line arguments.
//
// This function initializes the configurations to default values and then
// overrides them with any values provided as command-line arguments.
// The expected order of arguments is [rows] [columns] [filename] [generations].
// It uses `ParseLong` to safely parse numeric arguments (rows, columns,
// generations).
//
// Args:
//   config: A pointer to the config_t structure where the settings are be stored.
//   argc:   The number of command-line arguments.
//   args:   An array of pointers to the strings which are those arguments.
//
// Returns:
//   0 if the settings are successfully set.
//   1 if there is an error such as an invalid input or too many arguments.
int ConfigureGame(config_t* config, int argc, char* args[]) {
    config->rows = kDefaults.rows;
    config->cols = kDefaults.cols;
    config->filename = kDefaults.filename;
    config->generations = kDefaults.generations;

    if (argc > 5) {
        printf("Usage: life [rows] [columns] [filename] [generations]\n");
        return 1;
    }
    if (argc >= 2) {
        if (ParseLong(args[1], &config->rows) != 0) {
            printf("Error: Invalid input for rows, '%s'\n", args[1]);
            return 1;
        }
    }
    if (argc >= 3) {
        if (ParseLong(args[2], &config->cols) != 0) {
            printf("Error: Invalid input for columns, '%s'\n", args[2]);
            return 1;
        }
    }
    if (argc >= 4) {
        config->filename = args[3];
    }
    if (argc == 5) {
        if (ParseLong(args[4], &config->generations) != 0) {
            printf("Error: Invalid input for generations, '%s'\n", args[4]);
            return 1;
        }
    }
    return 0;
}

// Allocates and initializes a 2D grid of characters.
//
// Args:
//   rows: Number of rows in the grid.
//   cols: Number of columns in the grid.
//   ch: Character used to initialize each cell in the grid.
//
// Returns:
//   A pointer to the newly allocated 2D grid.
char** CreateCharGrid(size_t rows, size_t cols, char ch) {
    char** grid = malloc(sizeof(char*) * rows);

    for (size_t i = 0; i < rows; i++) {
        grid[i] = malloc(sizeof(char) * cols);
        memset(grid[i], ch, sizeof(char) * cols);
    }
    return grid;
}

// Creates the initial world state from a file.
//
// Reads the world configuration from the given file descriptor and
// initializes the world grid based on the file content.
//
// Args:
//   fd:     File descriptor for the file containing the initial world state.
//   config: Pointer to the configuration struct holding the dimensions and
//           other settings.
//
// Returns:
//   A pointer to the newly created world grid.
char** CreateWorld(FILE* fd, const config_t* config) {
    char** world = CreateCharGrid(config->rows + 2, config->cols + 2, '-');
    
    char* line = NULL;
    size_t n = 0;
    for (size_t i = kOffset; i <= config->rows; i++) {
        if (getline(&line, &n, fd) < 0) {
            break;
        }
        for (size_t j = kOffset; j <= MIN(config->cols, n); j++) {
            if (line[j - kOffset] != '*') {
                continue;
            }
            world[i][j] = line[j - kOffset];
        }
    }
    free(line);
    return world;
}

// Frees the memory allocated for a 2D grid.
//
// Args:
//   grid: Pointer to the 2D grid to be freed.
//   rows: Number of rows in the grid.
//
// Returns:
//   0 to indicate successful deallocation.
int FreeGrid(char** grid, size_t rows) {
    for (size_t i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
    return 0;
}

// Computes the next state of a cell.
//
// This function calculates the new state of a cell located at (x, y) in the
// grid. It counts the number of alive neighbors and applies the rules to
// determine the cell's next state. The cell's state changes based on the
// number of alive neighbors. Note: '*' for alive, '-' for dead
//
// Args:
//   world: A 2D array representing the game world where each cell contains 
//          either a live ('*') or a dead ('-') character.
//   y:     The y-coordinate (row index) of the cell.
//   x:     The x-coordinate (column index) of the cell.
//
// Returns:
//   The character representing the new state of the cell.
//
// Note:
//   This function assumes that (x, y) are valid coordinates in the world.
char ComputeCellState(const char** world, size_t y, size_t x) {
    int neighbor_count = 0;
    for (size_t i = y - kOffset; i <= y + kOffset; i++) {
        for (size_t j = x - kOffset; j <= x + kOffset; j++) {
            if (IsAlive(world[i][j]) && !(i == y && j == x)) {
                neighbor_count++;
            }
        }
    }

    char new_state = world[y][x];
    if (IsAlive(world[y][x])) {
        if (neighbor_count < 2 || neighbor_count > 3) {
            new_state = '-';
        }
    } else if (IsDead(world[y][x]) && neighbor_count == 3) {
        new_state = '*';
    }

    return new_state;
}

// Updates the state of the entire world for the next generation.
//
// This function creates a copy of the current state of the world, computes the new state
// for each cell using ComputeCellState, and updates the world with the new states.
// It ensures that the original world state is not modified during computation to
// prevent conflicts between reading the current state and writing the new state.
//
// Args:
//   world:  A pointer to a 2D array representing the current state of the game world.
//   config: A pointer to the config_t structure containing the game configuration,
//           including the number of rows, columns, and other settings.
void play(char** world, const config_t* config) {
    char** world_copy = CreateCharGrid(config->rows + 2, config->cols + 2, '-');
    for (size_t i = kOffset; i <= config->rows; i++) {
        memcpy(world_copy[i], world[i], sizeof(char) * (config->cols + 2));
    }

    for (size_t i = kOffset; i <= config->rows; i++) {
        for (size_t j = kOffset; j <= config->cols; j++) {
            char new_state = ComputeCellState((const char **)world_copy, i, j); 
            world[i][j] = new_state;
        }
    }
    FreeGrid(world_copy, config->rows + 2);
}

// Usage: life [rows] [columns] [filename] [generations]
int main(int argc, char* argv[]) {
    config_t config;

    if (ConfigureGame(&config, argc, argv) != 0) {
        return 1;
    }

    FILE* fd = fopen(config.filename, "r");
    if (!fd) {
        perror("Error: File open operation failed");
        return 1;
    }
    
    char** world = CreateWorld(fd, &config);
    fclose(fd);

    for (size_t gen = 0; gen <= config.generations; gen++) {
        PrintWorld((const char**)world, &config, gen);
        play(world, &config);
    }
    FreeGrid(world, config.rows + 2);
    return 0;
}