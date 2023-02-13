#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head = NULL;
struct node *tail = NULL;
struct node *temp = NULL;
int count = 0;

// Function to add a task to the list using the round robin algorithm.
void add(char* name, int priority, int burst, int tid){
    if (!head)  {
        head = malloc(sizeof(struct node));
        head->task = malloc(sizeof(Task));
        head->task->name = name;
        head->task->priority = priority;
        head->task->burst = burst;
        head->task->tid = tid;
        head->next = NULL;
        tail = head;
        count++;
    }
    else {
        temp = malloc(sizeof(struct node));
        temp->task = malloc(sizeof(Task));
        temp->task->name = name;
        temp->task->priority = priority;
        temp->task->burst = burst;
        temp->task->tid = tid;
        temp->next = NULL;
        tail->next = temp;
        tail = temp;
        count++;
    }
}

// scheduling function that will run each task in the list using the round robin algorithm. this algorithmn runs each tasks for a quantum and then moves on to the next task. If the task is not finished, it will be placed at the end of the list. The quantum is 10.
void schedule() {
    struct values process[count + 2];
    float time = 0;
    int cont_switch = 0;
    int quantum = QUANTUM;
    struct node *curr = head;
    int tid = 0;
    for (int i = 0; i < count; i++) {
        process[i].response = INT_MIN;
        process[i].wait = INT_MIN;
        process[i].turnaround = INT_MIN;
        process[i].burst = INT_MIN;
    }

    while (curr)    {
        tid = curr->task->tid;

        if (process[tid].burst == INT_MIN)
            process[tid].burst = curr->task->burst;

        if (process[tid].response == INT_MIN)
            process[tid].response = time;

        if (curr->task->burst >= quantum)    {
            run(curr->task, quantum);
            time += quantum;

            process[tid].turnaround = time;
            process[tid].wait = process[tid].turnaround - process[tid].burst;

            curr->task->burst -= quantum;
            if (curr->task->burst > 0)
                insertEnd(&tail, curr->task);
              
            temp = curr;
            curr = curr->next;
            delete(&head, temp->task);
            cont_switch++;
        }
        else    {
            run(curr->task, curr->task->burst);
            time += curr->task->burst;

            process[tid].turnaround = time;
            process[tid].wait = process[tid].turnaround - process[tid].burst;

            curr->task->burst = 0;
            temp = curr;
            curr = curr->next;
            delete(&head, temp->task);
            cont_switch++;
        }
        printf("Time is now: %0.0lf\n", time);
    }
    float CPU = (float)(time/(time + cont_switch));
    CPU *= 100;
    printf("CPU Utilization: %0.2f%%\n", CPU);

    // Time calculations
    printf("...|");
    for (int i = 1; i <= count; i++)    {
        printf(" T%d |", i);

    }
    printf("\n");

    // Turnaround
    printf("TAT|");
    for (int i = 1; i <= count; i++) {
        printf(" %d |", process[i].turnaround);
    }
    printf("\n");

    // Waiting time
    printf("WT |");
    for (int i = 1; i <= count; i++) {
        printf(" %d |", process[i].wait);
    }
    printf("\n");

    // Response time
    printf("RT |");
    for (int i = 1; i <= count; i++) {
        printf(" %d |", process[i].response);
    }
    printf("\n");
}