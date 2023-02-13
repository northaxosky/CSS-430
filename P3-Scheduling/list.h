/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node {
    Task *task;
    struct node *next;
};

struct values {
    int turnaround;
    int wait;
    int response;
    int burst;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
void insertEnd(struct node **tail, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);