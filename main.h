// Christian Catalano, Assignment 2, March 19th, 1120832
#ifndef SIMCPU
#define SIMCPU

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    FIRST, BEST, NEXT, WORST
} algo;

typedef struct node_Struct{
    int pid;
    int memSize;
    int memLocation;
    struct node_Struct* next;
} node;

/* Linked List Functions */
void freeList(node* list);
void freeNode(node* n);
void printList(node* list);
void push(node** list, int pid, int memSize, int memLocation);
node* pop(node** list);

node* createNode(int pid, int memSize, int memLocation);


/* Other Functions */
int min( int n1, int n2 );
int max( int n1, int n2 );

#endif /* SIMCPU */