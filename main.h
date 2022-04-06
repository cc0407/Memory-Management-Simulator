// Christian Catalano, Assignment 2, March 19th, 1120832
#ifndef SIMCPU
#define SIMCPU

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    FIRST, BEST, WORST, NEXT
} algo;

typedef struct node_Struct{
    int pid;
    int memSize;
    int memLocation;
    int swappedCount;
    int memTime;
    struct node_Struct* next;
} node;

node* readDataFromFile(char* filename);
void first(node* waitingList);
void best(node* waitingList);
void worst(node* waitingList);
void next(node* waitingList);


/* Linked List Functions */
void freeList(node* list);
void freeNode(node* n);
void printList(node* list);
void push(node** list, int pid, int memSize, int memLocation);
void pushNode(node** list, node* n);
node* pop(node** list);
node* popSpecific(node** list, node* remove);
int length(node* list);

node* createNode(int pid, int memSize, int memLocation);

void printDetails(node* memList, int memUsage, int cumulative);
int calculateHoles(node* list); // Calculates the total number of holes in memory
int largestHole(node* memList); // Returns the int value of the largest hole in memory
void ageNodes(node* memList); // Increases the age of all nodes by 1
node* removeOldest(memList); // Removes oldest node from memory and places into waiting queue

#endif /* SIMCPU */