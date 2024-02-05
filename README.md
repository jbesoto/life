# Conway's Game of Life (in C)

## Overview
This program simulates Conway's Game of Life, a cellular automaton where the evolution of a grid of cells is determined by a set of rules. The program reads an initial state from a file, evolves this state through a specified number of generations, and prints the state of the grid after each generation.

## Features
- Customizable grid size and number of generations through command-line arguments.
- Initial world state is loaded from a file.
- Option to specify custom file name for initial state.

## Compilation and Execution
To compile and run the program, use the following commands in your terminal:

```bash
gcc -o life life.c
./life [rows] [columns] [filename] [generations]
```

## Command-Line Arguments
1. `rows` (optional): Number of rows in the grid. Default is 10.
2. `columns` (optional): Number of columns in the grid. Default is 10.
3. `filename` (optional): Name of the file containing the initial state. Default is "life.txt".
4. `generations` (optional): Number of generations to simulate. Default is 10.

## File Format
The program expects the initial state to be provided in a text file. The file should contain a grid of characters where '*' represents a live cell and '-' represents a dead cell. The size of the grid in the file should match the `rows` and `columns` parameters.

## Functionality
- `IsAlive` and `IsDead` functions determine the state of a cell.
- `PrintWorld` function prints the current state of the world.
- `ParseLong` function safely parses numeric command-line arguments.
- `ConfigureGame` function sets the game configurations based on command-line arguments.
- `CreateCharGrid` and `FreeGrid` handle dynamic memory allocation and deallocation for the grid.
- `CreateWorld` initializes the world grid from the file.
- `ComputeCellState` calculates the next state of a cell based on its neighbors.
- `play` function updates the state of the world for the next generation.

## Notes
- Ensure that the input file exists and is formatted correctly before running the program.
- The program prints the state of the world for each generation, including the initial state (generation 0).