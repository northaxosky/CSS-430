#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define _GNU_SOURCE

#define MEMSIZE 80
#define MAXLINE 100

int main();
void run(char *line);
void firstFit(char name, int size);
void bestFit(char name, int size);
void worstFit(char name, int size);
