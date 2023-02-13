#include <stdlib.h>
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

// Schedule function that keeps track of time, turnaround time, wait time, and response time
void schedule(){
    struct values process[count + 1];
    double time = 0;
    int p = 1;

    struct node *curr = head;
    while (curr)    {
        //update values to be zero
        run(curr->task, curr->task->burst);
        time += curr->task->burst;
        process[p].turnaround = time;
        process[p].response = time - curr->task->burst;
        process[p].wait = time - curr->task->burst;
        curr = curr->next;
        p++;

        printf("Time is now: %0.0lf\n", time);
    }
    // CPU utilization calculations
    float CPU = (float)(time/(time + (count - 1)));
    CPU *= 100;
    printf("CPU Utilization: %0.2f%%\n", CPU);

    // Time per process calculations
    printf("...|");
    for (int i = 0; i < count; i++) {
        printf(" T%d |", i + 1);
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