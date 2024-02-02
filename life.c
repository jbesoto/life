#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


// Prints the current state of the world for a given generation.
//
// Args:
//   world:  A pointer to a 2D array of characters where each character represents
//           a cell in the world. '*' for a live cell and '-' for a dead cell.
//   config: A pointer to the config_t structure where the settings are stored.
//   gen:    The current generation number that is being printed.
void PrintWorld(const char** world, config_t* config, int gen) {
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

char** CreateCharGrid(int rows, int cols, char ch) {
    char** grid = malloc(sizeof(char*) * rows);

    for (size_t i = 0; i < rows; i++) {
        grid[i] = malloc(sizeof(char) * cols);
        memset(grid[i], ch, sizeof(char) * cols);
    }
    return grid;
}

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

int FreeGrid(char** world, size_t rows) {
    for (size_t i = 0; i < rows; i++) {
        free(world[i]);
    }
    free(world);
    return 0;
}

// life [rows] [columns] [filename] [generations]
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
        PrintWorld(world, &config, gen);
        play(world, &config);
    }
    FreeGrid(world, config.rows + 2);
    return 0;
}