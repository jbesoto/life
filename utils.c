/**
 * utils.c
 *
 * This file contains utility functions used across the Game of Life simulation project.
 * It includes implementations for parsing command-line arguments, creating and managing
 * grids or worlds, and other helper functionalities that support the main simulation.
 *
 * Author: Juan Diego Becerra
 * Date: 2024-03-16
 */

#include "utils.h"

/**
 * @brief Parses the given string as a long integer.
 *
 * Parses the given string `arg` as a long integer and stores the result in
 * `out` if successful. The function expects `arg` to represent a positive
 * integer using base 10 for the conversion.
 *
 * @param out A pointer to a `size_t` where the parsed value will be stored.
 * @param arg The string representation of the long integer to be parsed.
 * 
 * @return Returns 0 on successful parsing and storing of the long integer.
 *         Returns -1 if the parsing fails due to `arg` not being a valid
 *         positive integer or if any other error occurs during conversion
 *         (e.g., overflow).
 *
 * @note The function sets `errno` to non-zero if strtol encounters errors.
 */
int ParseLong(size_t* out, const char* arg) {
  char* end;
  long value = strtol(arg, &end, 10);
  if (errno != 0 || value < 1 || !out) {
    return -1;
  }

  *out = (size_t)value;
  return 0;
}

/**
 * @brief Creates and returns a 2D grid of characters.
 *
 * Creates and returns a pointer to a 2D grid of characters with the specified
 * number of `rows` and `cols`, each cell initialized to the character `ch`.
 * The function allocates memory for the grid and for each row individually.
 *
 * @param rows The number of rows in the grid.
 * @param cols The number of columns in the grid.
 * @param ch   The character to initialize each cell of the grid with.
 * 
 * @return Returns a pointer to the array of character pointers (rows) on 
 *         success. Returns NULL if memory allocation fails for the grid or any
 *         row.
 *
 * @note The caller is responsible for freeing the allocated memory for the
 *       grid and its rows after use. Memory allocation failure for any row 
 *       triggers cleanup of previously allocated rows and the grid itself
 *       before returning NULL.
 */
char** CreateCharGrid(size_t rows, size_t cols, char ch) {
  char** grid = malloc(sizeof(char*) * rows);
  if (!grid) {
    return NULL;
  }

  for (size_t i = 0; i < rows; i++) {
    grid[i] = malloc(sizeof(char) * cols);
    if (!grid[i]) {
      while (i > 0) {
        free(grid[--i]);
      }
      return NULL;
    }
    memset(grid[i], ch, sizeof(char) * cols);
  }
  
  return grid;
}

/**
 * @brief Frees the memory allocated for a 2D grid of characters.
 *
 * Iterates over each row of the grid to free the memory allocated for the row,
 * and then frees the memory allocated for the grid itself. This function
 * should be called to clean up memory allocated by `CreateCharGrid`.
 *
 * @param grid A pointer to the array of character pointers (rows) to be freed.
 * @param rows The number of rows in the grid, which were allocated.
 *
 * @note It is safe to call this function with a NULL pointer or with a `rows`
 *       value of 0. However, the caller must ensure that the grid and its rows
 *       were allocated dynamically using `malloc` or similar allocation
 *       functions.
 */
void FreeGrid(char** grid, size_t rows) {
  if (grid) {
    for (size_t i = 0; i < rows; i++) {
      free(grid[i]);
    }
    free(grid);
  }
}