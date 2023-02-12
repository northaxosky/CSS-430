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

// Function to add a task to the list using the shortest job first algorithm. The list is sorted by burst time where the shortest burst time is first. Insert the tasks sorted by burst time. If two tasks have the same burst time, then the task with the highest priority is first. If the priority is same sort by TID.
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

    //compare the head burst time to the new burst time
    if (head->task->burst > burst) {
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

    while (curr->next && curr->next->task->burst < burst) {
        curr = curr->next;
    }

    //if the burst time is the same, then compare the priority
    if (curr->next && curr->next->task->burst == burst) {
        while (curr->next && curr->next->task->priority < priority) {
            curr = curr->next;
        }
    }

    //if priority is the same, then compare the TID
    if (curr->next && curr->next->task->priority == priority) {
        while (curr->next && curr->next->task->tid < priority) {
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


void schedule() {
    struct values process[count];
    double time = 0;
    int p = 0;

    struct node *curr = head;
    while (curr)    {
        //update values to be zero
        process[p].turnaround = 0;
        process[p].response = 0;
        process[p].wait = 0;
        run(curr->task, curr->task->burst);
        time += curr->task->burst;
        process[p].turnaround = time;
        process[p].response = time - curr->task->burst;
        process[p].wait = time - curr->task->burst;
        curr = curr->next;
        p++;

        printf("Time is now: %0.0lf\n", time);
    }
}
