# Sudoku verifier and solver

Works on sudoku puzzles of any size.
Uses multiple threads to check if a puzzle is valid.

For puzzles that have any "0"s, tries to find a valid number for the 0. Can solve simple puzzles where no backtracking is required.

2x2 puzzle

```
3 0 | 0 1
2 1 | 0 0
---------
0 0 | 0 2
4 2 | 1 0
```
Finds grid[1][2] as 4, grid[4][4] as 3
```
3 4 | 0 1
2 1 | 0 0
---------
0 0 | 0 2
4 2 | 1 3
```
Finds grid[1][3] ad 2, grid[3][1] as 1, grid[3][2] as 3
```
3 4 | 2 1
2 1 | 0 0
---------
1 3 | 0 2
4 2 | 1 3
```
Finds grid[2][4] as 4, grid[3][3] ad 4
```
3 4 | 2 1
2 1 | 0 4
---------
1 3 | 4 2
4 2 | 1 3
```
Finds grid[2][3] as 3
```
3 4 | 2 1
2 1 | 3 4
---------
1 3 | 4 2
4 2 | 1 3
```

The current version would not be able to solve a more complex puzzle, such as 
```
3 0 | 0 0
2 1 | 0 0
---------
0 0 | 0 0
4 2 | 1 0
```