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
void pushHead(node** list, node* n);
void insertAfter(node** list, node* before, node* new);
void pushNode(node** list, node* n);
node* pop(node** list);
node* popSpecific(node** list, node* remove);
int length(node* list);

node* createNode(int pid, int memSize, int memLocation);

void printDetails(node* memList, int memUsage, int cumulative);
int calculateHoles(node* list); // Calculates the total number of holes in memory
int largestHole(node* memList); // Returns the int value of the largest hole in memory
void ageNodes(node* memList); // Increases the age of all nodes by 1
node* removeOldest(node** memList); // Removes oldest node from memory and places into waiting queue
int calculateMemUsage(node* list);

bool insertFirst(node** list, node* n); // Attempts to move n into the first hole it can find
bool insertBest(node** list, node* n); // Attempts to move n into the smallest hole it can find that fits
bool insertWorst(node** list, node* n); // Attempts to move n into the largest hole it can find that fits
bool insertNext(node** list, node* n); // Attempts to move n into the next hole from the last placement

#endif /* SIMCPU */