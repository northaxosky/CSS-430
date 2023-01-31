// Sudoku puzzle verifier and solver
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// takes puzzle size and grid[][] representing sudoku puzzle
// and tow booleans to be assigned: complete and valid.
// row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
// has grid[1][1] as the top-left element and grid[9]9] as bottom right
// A puzzle is complete if it can be completed with no 0s in it
// If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
// to psize For incomplete puzzles, we cannot say anything about validity
int sudokuSize;
bool isValid;
bool isComplete; 


typedef struct {
    int row;
    int column;
    int **grid;
} parameters;


// Check if row is valid and completed
void *row_worker(void *param) {
  parameters *p = (parameters *) param;
  int row = p->row;
  int psize = sudokuSize;
  int **grid = p->grid;

  int *row_array = (int *) malloc((psize + 1) * sizeof(int));
  for (int i = 1; i <= psize; i++) {
    row_array[i] = 0;
  }
  for (int col = 1; col <= psize; col++) {
    if (grid[row][col] == 0) {
      isComplete = false;
    } else {
      row_array[grid[row][col]]++;
    }
  }
  for (int i = 1; i <= psize; i++) {
    if (row_array[i] != 1) {
      isValid = false;
    }
  }
  free(row_array);
  return NULL;
}

// Check if column is valid and completed
void *column_worker(void *param) {
  parameters *p = (parameters *) param;
  int column = p->column;
  int psize = sudokuSize;
  int **grid = p->grid;
  int *column_array = (int *) malloc((psize + 1) * sizeof(int));
  for (int i = 1; i <= psize; i++) {
    column_array[i] = 0;
  }
  for (int row = 1; row <= psize; row++) {
    if (grid[row][column] == 0) {
      isComplete = false;
    } else {
      column_array[grid[row][column]]++;
    }
  }
  for (int i = 1; i <= psize; i++) {
    if (column_array[i] != 1) {
      isValid = false;
    }
  }
  free(column_array);
  return NULL;
}

// Check if subsquare is valid and completed
void *square_worker(void *param) {
  parameters *p = (parameters *) param;
  int row = p->row;
  int column = p->column;
  int psize = sudokuSize;
  int **grid = p->grid;
  int *square_array = (int *) malloc((psize + 1) * sizeof(int));
  for (int i = 1; i <= psize; i++) {
    square_array[i] = 0;
  }
  int size = (int)(sqrt(psize));
  for (int i = row; i < row + size; i++) {
    for (int j = column; j < column + size; j++) {
      if (grid[i][j] == 0) {
        isComplete = false;
      } else {
        square_array[grid[i][j]]++;
      }
    }
  }
  for (int i = 1; i <= psize; i++) {
    if (square_array[i] != 1) {
      isValid = false;
    }
  }
  free(square_array);
  return NULL;
}

void checkPuzzle(int psize, int **grid) {
    // Create array of threads
    pthread_t *threads = malloc((psize + 2) * sizeof(pthread_t));
    int count = 2;

    // Create parameters for row and column threads
    parameters * row_param = malloc(sizeof(parameters));
    row_param->row = 1;
    row_param->column = 1;
    row_param->grid = grid;
    parameters * column_param = malloc(sizeof(parameters));
    column_param->row = 1;
    column_param->column = 1;
    column_param->grid = grid;

    pthread_t row_thread;
    pthread_create(&row_thread, NULL, row_worker, (void *) row_param);
    pthread_t column_thread;
    pthread_create(&column_thread, NULL, column_worker, (void *) column_param);

    threads[0] = column_thread;
    threads[1] = row_thread;
    
    // Iterate through subsquares to create subsquare threads
    int size = (int)(sqrt(psize));
    for (int row = 1; row < size; row += (size + 1))   {
      for (int col = 1; col < size; col += (size + 1))  {
        // create parameters for each thread
        parameters * param = malloc(sizeof(parameters));
        param->row = row;
        param->column = col;
        param->grid = grid;

        // Create a thread and add its id to the array
        pthread_t t;
        threads[count] = t;
        pthread_create(&t, NULL, square_worker, (void *) param);
        pthread_join(t, NULL);
        count++;

      }
    }

    for (int i = 0; i < 2; i++) {
      pthread_join(threads[i], NULL);
    }
}

bool isSafe(int grid[sudokuSize][sudokuSize], int row, int col, int num) {
  for (int x = 0; x < sudokuSize; x++) {
    if (grid[row][x] == num) {
      return false;
    }
  }

  for (int x = 0; x < sudokuSize; x++) {
    if (grid[x][col] == num) {
      return false;
    }
  }
  int size = (int)(sqrt(sudokuSize));
  int startRow = row - row % size;
  int startCol = col - col % size;

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (grid[i + startRow][j + startCol] == num) {
        return false;
      }
    }
  }

  return true;
}

bool solveSudoku(int grid[sudokuSize][sudokuSize], int row, int col) {
  if (row == sudokuSize - 1 && col == sudokuSize) {
    return true;
  }
  if (col == sudokuSize) {
    row++;
    col = 0;
  }

  if (grid[row][col] > 0) {
    return solveSudoku(grid, row, col + 1);
  }

  for (int num = 1; num <= sudokuSize; num++) {
    if (isSafe(grid, row, col, num)) {
      grid[row][col] = num;
      if (solveSudoku(grid, row, col + 1)) {
        return true;
      }
    }
    grid[row][col] = 0;
  }
  return false;
}

// Function to convert read values from int**grid and put them in a int[][]array where the values start at 0
void convertGrid(int psize, int **grid, int array[psize][psize]) {
  for (int row = 1; row <= psize; row++) {
    for (int col = 1; col <= psize; col++) {
      array[row - 1][col - 1] = grid[row][col];
    }
  }
}

// Function to print the new array
void printArray(int psize, int array[psize][psize]) {
  for (int row = 0; row < psize; row++) {
    for (int col = 0; col < psize; col++) {
      printf("%d ", array[row][col]);
    }
    printf("\n");
  }
}



// takes filename and pointer to grid[][]
// returns size of Sudoku puzzle and fills grid
int readSudokuPuzzle(char *filename, int ***grid) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  int psize;
  fscanf(fp, "%d", &psize);
  int **agrid = (int **)malloc((psize + 1) * sizeof(int *));
  for (int row = 1; row <= psize; row++) {
    agrid[row] = (int *)malloc((psize + 1) * sizeof(int));
    for (int col = 1; col <= psize; col++) {
      fscanf(fp, "%d", &agrid[row][col]);
    }
  }
  fclose(fp);
  *grid = agrid;
  return psize;
}

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int **grid) {
  printf("%d\n", sudokuSize);
  for (int row = 1; row <= sudokuSize; row++) {
    for (int col = 1; col <= sudokuSize; col++) {
      printf("%d ", grid[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int** grid) {
  for (int row = 1; row <= sudokuSize; row++) {
    free(grid[row]);
  }
  free(grid);
}

// expects file name of the puzzle as argument in command line
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: ./sudoku puzzle.txt\n");
    return EXIT_FAILURE;
  }
  int **grid = NULL;
  // find grid size and fill grid
  sudokuSize = readSudokuPuzzle(argv[1], &grid);
  // Create column and row threads threads
  isComplete = true;
  isValid = true;
  checkPuzzle(sudokuSize, grid);
  printf("Complete puzzle? ");
  printf(isComplete ? "true\n" : "false\n");
  if (isComplete) {
    printf("Valid puzzle? ");
    printf(isValid ? "true\n" : "false\n");
  }
  printSudokuPuzzle(grid);

  int array[sudokuSize][sudokuSize];
  convertGrid(sudokuSize, grid, array);
  if (!isComplete)  {
     printf("Solving...\n");
    if (!solveSudoku(array, 0, 0))
      printf("No solution");
    else
      printArray(sudokuSize, array);
  }
  printf("\n\n=============================\n");
  deleteSudokuPuzzle(grid);
  return EXIT_SUCCESS;
}