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

// Function to add a task to the list by comparing the priority of the tasks. The list is sorted by priority where the highest priority is first. Insert the tasks sorted by priority. If two tasks have the same priority, then the task with the shortest burst time is first. If the burst time is same sort by TID.
void add(char *name, int priority, int burst) {
    struct node* curr = head;
    if (!head)  {
        head = malloc(sizeof(struct node));
        head->task = malloc(sizeof(Task));
        head->task->name = name;
        head->task->priority = priority;
        head->task->burst = burst;
        head->next = NULL;
        tail = head;
        count++;
        return;
    }

    //compare the head priority to the new priority
    if (head->task->priority < priority) {
        temp = malloc(sizeof(struct node));
        temp->task = malloc(sizeof(Task));
        temp->task->name = name;
        temp->task->priority = priority;
        temp->task->burst = burst;
        temp->next = head;
        head = temp;
        count++;
        return;
    }

    while (curr->next && curr->next->task->priority > priority) {
        curr = curr->next;
    }

    //if the priority is the same, then compare the burst time
    if (curr->next && curr->next->task->priority == priority) {
        while (curr->next && curr->next->task->name < name && curr->next->task->priority == priority) {
            curr = curr->next;
        }
    }

    temp = malloc(sizeof(struct node));
    temp->task = malloc(sizeof(Task));
    temp->task->name = name;
    temp->task->priority = priority;
    temp->task->burst = burst;
    temp->next = curr->next;
    curr->next = temp;
    count++;
}

// Schedule algorithm
void schedule() {
    struct values process[count];
    double time = 0;

    struct node *curr = head;
    while (curr)    {
        run(curr->task, curr->task->burst);
        time += curr->task->burst;
        curr = curr->next;

        printf("Time is now: %0.0lf\n", time);
    }

    // CPU utilizations calculations
    float CPU = (float)(time/(time + (count -1)));
    CPU *= 100;
    printf("CPU Utilization: %0.2f%%\n", CPU);
}