#include <stdio.h>
#include <stdlib.h>

// Ensures that world traversal is kept within the valid cells
static const size_t kOffset = 1;


// Prints the current state of the game board for a given generation.
//
// Args:
//   world: A pointer to a 2D array of characters where each character represents
//          a cell in the game. '*' for a live cell and '-' for a dead cell.
//   cols:  The number of columns in the game board.
//   rows:  The number of rows in the game board.
//   gen:   The current generation number that is being printed.
void PrintWorld(char** world, size_t cols, size_t rows, int gen) {
    printf("Generation %d:\n", gen);

    for (size_t i = 0 + kOffset; i <= rows; i++) {
        for (size_t j = 0 + kOffset; j <= cols; j++) {
            printf("%c", world[i][j]);
            if (j == cols) {
                putchar('\n');
            }
        }
    }
    printf("================================\n");
}

// life [rows] [columns] [filename] [generations]
int main(int argc, char* argv[]) {
    // Parse user arguments
    
    // Set game settings

    // Read the file and create world

    // Print gen 0

    // Play game for `gens` generations
    return 0;
}