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

// Function to add a task to the list using the round robin algorithm.
void add(char* name, int priority, int burst){
    if (!head)  {
        head = malloc(sizeof(struct node));
        head->task = malloc(sizeof(Task));
        head->task->name = name;
        head->task->priority = priority;
        head->task->burst = burst;
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
        temp->next = NULL;
        tail->next = temp;
        tail = temp;
        count++;
    }
}

// scheduling function that will run each task in the list using the round robin algorithm. this algorithmn runs each tasks for a quantum and then moves on to the next task. If the task is not finished, it will be placed at the end of the list. The quantum is 10.
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