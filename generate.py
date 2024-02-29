#!/usr/bin/env python3

"""
Generates an initial board for the Game of Life.

This script creates a ROW x COLUMN grid of cells where each cell is either alive ('*')
or dead (' '). The initial state of each cell is determined randomly with a
probability of 5% for a cell to be alive and 95% to be dead. The resulting grid
is saved to a text file named 'life.txt'.

Attributes:
    rows (int): Number of rows in the grid.
    cols (int): Number of columns in the grid.
    file_path (str): Path to the output text file.
"""

import sys

import numpy as np

def main():
    if len(sys.argv) > 4:
        print("Usage: ./generate [ROWS] [COLUMNS]")
        exit(1)

    rows, cols = sys.argv[1:]
    file_path = './life.txt'

    grid = np.random.choice(['*', ' '], size=(rows, cols), p=[0.05, 0.95])

    grid_str = '\n'.join(''.join(row) for row in grid)

    with open(file_path, 'w') as file:
        file.write(grid_str)

if __name__ == '__main__':
    main()
