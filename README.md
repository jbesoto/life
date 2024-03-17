# Conway's Game of Life

## Introduction

Conway's [*Game of Life*](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) is a cellular automaton devised by the British mathematician John Horton Conway in 1970. It's not a game in the conventional sense, but rather a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. Participants create an initial configuration and watch how it evolves over time.

This project is a C language implementation of the game, designed to run in a command-line environment. It adheres to the original rules set by John Conway where cells in a grid live, die, or multiply, leading to a variety of patterns that emerge throughout the course of the simulation.

![Game Demonstration GIF](assets/demo.gif)

## Features

This implementation of the game boasts a variety of features that allow users to customize and interact with the game. These features are designed to enhance user experience and provide flexibility in how the game is played and observed. Here’s what you can expect from this program:

- **Command-Line Interface**: A user-friendly command-line interface that makes it easy to start the game with various configurations directly from your terminal.

- **Customizable Grid Sizes**: You can set the game's grid to any size you want, as long as it doesn't exceed your console's width and height, providing a versatile playing field for the cells.

- **File Input for Initial State**: The program accepts a file containing the initial state of the world, allowing you to define complex cell configurations without manual input.

- **Variable Generations**: Choose how many generations you'd like to simulate, observing the life cycle of cells over a short burst or an extended period.

- **Animated World State Display**: The state of the game world is displayed in the console after each generation, providing a clear and immediate visual of life's progression.

## Installation

Before you begin, ensure that you have a C compiler (such as `gcc`) and `make` utility installed on your system.

1. **Clone the Repository**:
   ```
   git clone https://github.com/jbesoto/life.git
   cd life
   ```

2. **Compile the Code**:
   Use the provided Makefile to compile the source code:
   ```Makefile
   make
   ```
   This command should generate an executable named `life`.

3. **Verify the Installation**:
   Check if the `life` executable was created successfully:
   ```
   ./life --help
   ```
   If you see the usage instructions, the installation is successful.

## Usage

After installing, you can run the game as follows:
```
./life [options]
```

### Options:

- `-r, --rows NUM`: Set the number of rows for the game grid.
- `-c, --columns NUM`: Set the number of columns for the game grid.
- `-f, --filename FILENAME`: Specify the path to a file with an initial world state.
- `-n, --generations NUM`: Set how many generations to simulate.
- `--debug`: Disables the animation to keep the game state visible between generations.
- `-h, --help`: Print the help message with usage instructions.

#### Note

If no options are provided, the game will use default settings: 10x10 grid, `life.txt` for the initial state, and 10 generations.

## Try It Out

There are a number of pre-created worlds located in the `tests/` folder that you can try out. Additionally, a Python script `generate.py` has been included in the `scripts/` folder to help you generate random worlds.

![Example Simulation GIF](assets/example.gif)
