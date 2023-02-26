/*
This project asks you to manage allocations within a memory pool of size MEMSIZE.  Your program support six different requests:

1. Allocate N bytes for a process using one of the 3 allocation algorithms

2. Free all allocations for a given process

3. Show the status of the memory pool – allocated and free blocks

4. Read a script – a sequence of commands from a file, and execute them

5. Compact the allocations, making them into one contiguous block.  (This somewhat resembles the operation of a mark-sweep garbage collector in C#)

6. Exit

MEMSIZE has a value of 80.  So we can think of the memory pool as holding 80 bytes.  (If you prefer, you can think of it as holding 80 KBytes, where allocations are made in multiples of 1 KByte)

Processes are named as a single letter, A thru Z.

Here is an example of the Show command after some allocations and frees:

AAAAAAAAAA..........BBBBBBBBBBFFFFFFFFFFFFFFFGGG..CCCCCH..............DDDDD.....

(MEMSIZE was chosen as 80 so the display would fit nicely onto our console screen)

The 6 command formats are as follows:

A  <name>  <size>  <algo>
Allocate <size> bytes for process <name> using algorithm <algo>.  <algo> can be any of F for First-Fit, B for Best-Fit or W for Worst-Fit.
Eg:            A   P   20  F               Allocate 20 bytes for process P using First-Fit
Eg:            A   X   14  B               Allocate 14 bytes for process X using Best-Fit

F  <name>
Free all the allocations owned by <name>
Eg:            F  P                             Free all allocations owned by process P

S
Show the state of the memory pool

R  <file>
Read the script in the file called <file> and execute each command.
Eg:            R   MEMO.TXT

C
Compact the memory pool, sliding all allocations to lower addresses so they become one contiguous block, and so that all the free space lies to the right as one contiguous block

E
exit

Here is an example script:

A  A  10  F

A  X  10  F

A  B  10  F

A  X  20  F

A  C   5  F

A  X  15  F

A  D   5  F

F  X

E
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MEMSIZE 80
char memory[MEMSIZE];

// main functions to take in input of R filename.txt, open up the file, and run the commands in the file
int main()  {
    // Ask for user to input command (of file name)
    char command[100];
    printf("Allocator> ");
    fgets(command, sizeof(command), stdin);

    // Initialize memory pool
    for (int i = 0; i < MEMSIZE; i++)  {
        memory[i] = '.';
    }

    // If command is R, then open up the file and run the commands in the file
    if (command[0] == 'R')  {
        // Open up the file
        FILE *fp;
        fp = fopen(command, "r");

        // Run the commands in the file
        char line[100];
        while (fgets(line, sizeof(line), fp))  {
            // Run the commands in the file
            printf("Allocator> %s\n", line);
            run(line);

        }
    }
}

// function to take in input of A name size algo, and allocate the size of memory for the process, also do the other commands
void run(char *line)  {
    // If command is A, then allocate the size of memory for the process
    if (line[0] == 'A')  {
        // Get the name of the process
        char name = line[2];

        // Get the size of the memory
        char size = line[4];

        // Get the algorithm
        char algo = line[6];

        // Allocate the size of memory for the process
        if (algo == 'F')  {
            // First-Fit
            firstFit(name, size);
        }
        else if (algo == 'B')  {
            // Best-Fit
            bestFit(name, size);
        }
        else if (algo == 'W')  {
            // Worst-Fit
            worstFit(name, size);
        }
    }
    // If command is F, free all the processes owned by the name
    else if (line[0] == 'F')  {
        // Get the name of the process
        char name = line[2];

        // Free all the processes owned by the name
        for (int i = 0; i < MEMSIZE; i++)  {
            if (memory[i] == name)  {
                memory[i] = '.';
            }
        }
    }
    // if the command is S, then show the status of the memory pool
    else if (line[0] == 'S')  {
        // Show the status of the memory pool
        for (int i = 0; i < MEMSIZE; i++)  {
            printf("%c", memory[i]);
        }
        printf("\n");
    }
    // if the command is C, then compact the memory pool
    else if (line[0] == 'C')  {
        // Compact the memory pool
        for (int i = 0; i < MEMSIZE; i++)  {
            if (memory[i] == '.')  {
                for (int j = i; j < MEMSIZE; j++)  {
                    if (memory[j] != '.')  {
                        memory[i] = memory[j];
                        memory[j] = '.';
                        break;
                    }
                }
            }
        }
    }
}

// Function for First-Fit algorithm
void firstFit(char name, int size)  {

    for (int i = 0; i < MEMSIZE; i++)  {
        if (memory[i] == '.')  {
            memory[i] = name;
        }
    }
}

void bestFit(char name, int size)  {
    // Allocate the size of memory for the process
    for (int i = 0; i < MEMSIZE; i++)  {
        if (memory[i] == '.')  {
            memory[i] = name;
        }
    }
}

void worstFit(char name, int size)  {
    // Allocate the size of memory for the process
    for (int i = 0; i < MEMSIZE; i++)  {
        if (memory[i] == '.')  {
            memory[i] = name;
        }
    }
}
