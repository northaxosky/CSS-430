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

    //if the priority is the same, then compare the name
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

    //if last node, then update tail
    if (curr->next->next == NULL) {
        tail = curr->next;
    }
}

// invoke the scheduler which uses round robin algorithm to schedule the tasks. If a task is finished, then it is removed from the list. If a task is not finished, then it is moved to the end of the list.
void schedule() {
    float time = 0;
    int cont_switch = 0;
    int quantum = QUANTUM;
    struct node *curr = head;

    while (curr)    {
        if (curr->task->burst >= quantum)    {
            run(curr->task, quantum);
            time += quantum;
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
}