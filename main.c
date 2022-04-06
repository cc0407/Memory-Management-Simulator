// Christian Catalano, Assignment 2, March 19th, 1120832
#include "main.h"

int main(int argc, char* argv[]) {
    algo algoType;
    node* memList = NULL;
    int iterationCount = 0;
    int memUsage = 0;
    int numProcesses = 0;
    int numHoles = 0;
    int cumulativeMemUsage = 0;
    int cumulativeProcessCount = 0;
    int cumulativeHoleCount = 0;

    /* Parameter Validation */
    if(argc != 3) {
        printf("Usage: ./hole [inputfile] <first|best|worst|next>\n");
        return 1;
    }

    /* Parse Input Parameters */
    if(strcmp(argv[2], "first") == 0) {
        algoType = FIRST;
    } else if(strcmp(argv[2], "best") == 0) {
        algoType = BEST;
    } else if(strcmp(argv[2], "worst") == 0) {
        algoType = WORST;
    } else if(strcmp(argv[2], "next") == 0) {
        algoType = NEXT;
    } else {
        printf("Usage: ./hole [inputfile] <first|best|worst|next>\n");
        return 1;
    }


    /* Create list from file*/
    node* waitingList = readDataFromFile(argv[1]);
    if(waitingList == NULL) {
        return 0;
    }

    /* Main Loop */
    node* tempNode;
    node* removeNode;
    bool successFlag;
    while(length(waitingList) > 0) {
        successFlag = false;
        ageNodes(memList);
        if(length(memList) == 0) { // No location in memory yet
            tempNode = pop(&waitingList);
            tempNode->memLocation = 0;
            tempNode->memTime = 0;
            tempNode->next = NULL;
            pushNode(&memList, tempNode); // Add process in to memlocation 0
        }
        else {
            tempNode = pop(&waitingList);
            tempNode->memTime = 0; // Resetting time in memory for this process

            while (!successFlag) {
                switch(algoType) {
                    case FIRST:
                        successFlag = insertFirst(&memList, tempNode);
                        break;
                    case BEST:
                        break;
                    case WORST:
                        break;
                    case NEXT:
                        break;
                }
                if(!successFlag) { // Remove one process from memory and see if theres enough space now

                    removeNode = removeOldest(&memList);
                    removeNode->swappedCount = removeNode->swappedCount + 1;

                    if(removeNode->swappedCount >= 3) { // Process is "finished", remove it fully
                        free(removeNode);
                    }
                    else { // Remove details about location of current process, and put back into waiting queue
                        removeNode->memLocation = -1;
                        pushNode(&waitingList, removeNode);
                    }
                }
            }

        }

        iterationCount++;
        memUsage = calculateMemUsage(memList);
        cumulativeMemUsage += memUsage;
        numHoles = calculateHoles(memList);
        cumulativeHoleCount += numHoles;
        numProcesses = length(memList);
        cumulativeProcessCount += numProcesses;
        printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n", numProcesses, numHoles, memUsage, cumulativeMemUsage/iterationCount);
    }
    printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%mem = %d\n", 
        iterationCount, (float)(cumulativeProcessCount)/(float)(iterationCount), (float)(cumulativeHoleCount)/(float)(iterationCount), cumulativeMemUsage/iterationCount);

    freeList(memList);
    freeList(waitingList);
    return 0;
}

void freeList(node* list) {
    node* tempPtr;
    while(list != NULL) {
        tempPtr = list->next;
        free(list);
        list = tempPtr;
    }
}

void printList(node* list) {
    node* tempPtr = list;
    printf("---\n");
    while(tempPtr != NULL) {
        printf("%d: %d in location [%d]\n", tempPtr->pid, tempPtr->memSize, tempPtr->memLocation);
        tempPtr = tempPtr->next;
    }
    printf("---\n");
}

void push(node** list, int pid, int memSize, int memLocation) {
    node* head = *list;
    node* newNode = createNode(pid, memSize, memLocation);
    if(head == NULL) { // List is empty, update head with current details
        *list = newNode;
        return;
    }

    node* current = head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

// Inserts a new node after [before] in the [list]
void insertAfter(node** list, node* before, node* new) {
    node* tempNode = *list;
    node* next;

    while(tempNode != NULL && tempNode != before) {
        tempNode = tempNode->next;
        next = tempNode->next;
    }

    if(tempNode == NULL) { // Node was not found in list
        return;
    }

    // Insert new node in between before and next
    tempNode->next = new;
    new->next = next;

}

void pushNode(node** list, node* n) {
    node* head = *list;
    if(head == NULL) { // List is empty, update head with current details
        *list = n;
        return;
    }

    // Navigate to end of linked list
    node* current = head;
    while(current->next != NULL) {
        current = current->next;
    }

    current->next = n; // append node to end of list
    n->next = NULL; // set end of list to null
}

node* pop(node** list) {
    node* head = *list;
    *list = head->next;
    return head;
}

// Deletes a specific node from the list
node* popSpecific(node** list, node* remove) {
    node* tempNode = *list;
    node* prev = NULL;

    while(tempNode != NULL && tempNode != remove) {
        prev = tempNode;
        tempNode = tempNode->next;
    }

    if(tempNode == NULL) { // Node was not found in list
        return NULL;
    }

    if(prev == NULL) { // Node to be removed is first in the list
        return pop(list);
    }
    else { // Node exists somewhere in the list
        prev->next = tempNode->next;
        return tempNode;
    }

}

node* createNode(int pid, int memSize, int memLocation) {
    node* newNode = malloc(sizeof(node));
    newNode->pid = pid;
    newNode->memSize = memSize;
    newNode->memLocation = memLocation;
    newNode->swappedCount = 0;
    newNode->memTime = -1;
    newNode->next = NULL;

    return newNode;
}


node* readDataFromFile(char* filename) {
    node* list = NULL;
    FILE *inFile;
    int tempChar;
    char tempString[100];
    int fileLen;

    // Prepare stdin and input file
    fflush(stdin);
    if(( inFile = fopen(filename, "r") ) == NULL)
        return NULL;

    // Find amt of lines in input file
    fileLen = 0;
    for (tempChar = getc(inFile); tempChar != EOF; tempChar = getc(inFile))
        if(tempChar == '\n')
            fileLen++;

    fseek(inFile, 0, SEEK_SET); // go back to start of file

    // Ingest data for each line in input file
    int pid;
    int memSize;
    for(int i = 0; i <= fileLen; i++) {
        if( fscanf(inFile, " %d %d ", &pid, &memSize) != 2) {
            printf("Error reading input file on line %d\nPlease verify input file is in the format:\n   [pid] [mem size]\n", i + 1);
            freeList(list);
            return NULL;
        }
        push(&list, pid, memSize, -1);
    }

    fclose(inFile);
    return list;
}

int length(node* list) {
    if(list == NULL) {
        return 0;
    }

    int count = 0;
    node* tempPtr = list;
    while(tempPtr != NULL) {
        count++;
        tempPtr = tempPtr->next;
    }
    return count;
}

//pid loaded, #processes = 5, #holes = 3, %memusage = 41, cumulative %mem = 40
void printDetails(node* memList, int memUsage, int cumulative) {
    int numHoles = calculateHoles(memList);
    printf("pid loaded, #processes = %d, #holes = %d, %%memusage = %d, cumulative %%mem = %d\n", length(memList), numHoles, memUsage, cumulative);
}

int calculateHoles(node* list) {
    int listLen = length(list);
    int holes = 0;
    node* tempPtr = list;

    if(listLen == 0) // No processes in memory, entire thing is a hole
        return 1;
    else if(listLen == 1) // One process in memory, if its less than memory available there is one hole
        return (tempPtr->memSize < 1024);
    
    int rightAddr;
    int leftAddr;
    while(tempPtr != NULL) { // Calculate holes for every pair of loaded memory
        leftAddr = tempPtr->memLocation + tempPtr->memSize;
        if(tempPtr->next == NULL)
            rightAddr = 1024;
        else
            rightAddr = tempPtr->next->memLocation;

        if(leftAddr < rightAddr) // If the two processes are not side by side
            holes++;

        tempPtr = tempPtr->next;
    }

    return holes;
}

int largestHole(node* memList) {
    int listLen = length(memList);
    int largest = 0;
    node* tempPtr = memList;

    if(listLen == 0) // No processes in memory, entire thing is a hole
        return 1024;
    else if(listLen == 1) // One process in memory, if its less than memory available there is one hole
        return (1024 - tempPtr->memSize);
    
    while(tempPtr->next != NULL) { // Calculate holes for every pair of loaded memory
        if((tempPtr->memLocation + tempPtr->memSize) < tempPtr->next->memLocation) // If the two processes are not side by side
            if(largest < (tempPtr->next->memLocation - (tempPtr->memLocation + tempPtr->memSize)) ) // If this hole is larger than current hole, update it
                largest = tempPtr->next->memLocation - (tempPtr->memLocation + tempPtr->memSize);
        tempPtr = tempPtr->next;
    }

    if( 1024 - (tempPtr->memLocation + tempPtr->memSize) > largest) // Calculate distance from end of last process to end of 1024 block
        largest = 1024 - (tempPtr->memLocation + tempPtr->memSize);

    return largest;
}


// Increases the age of all nodes by 1
void ageNodes(node* memList) {
    node* tempPtr = memList;
    while(tempPtr != NULL) {
        tempPtr->memTime = tempPtr->memTime + 1;
        tempPtr = tempPtr->next;
    } 
} 

// Removes oldest node from memory and places into waiting queue
node* removeOldest(node** memList) {
    node* removeNode;
    node* tempPtr = *memList;
    int oldest = 0;

    // First pass, find oldest node
    while(tempPtr != NULL) {
        if(tempPtr->memTime > oldest) {
            oldest = tempPtr->memTime;
            removeNode = tempPtr;
        }

        tempPtr = tempPtr->next;
    }

    return popSpecific(memList, removeNode);
} 

// Attempts to move n into the first hole it can find
bool insertFirst(node** list, node* n) {
    int listLen = length(*list);
    int largest = 0;
    node* tempPtr = *list;

    if(listLen == 0) { // No processes in memory, entire thing is a hole
        n->memLocation = 0;
        pushNode(list, n);
        return true;
    }
    
    int leftAddr = 0; // the address in memory of the end of memory block
    int rightAddr = 0; // the address in memory of the start of the next memory block
    while(tempPtr != NULL) { // Find first hole that is large enough
        leftAddr = tempPtr->memLocation + tempPtr->memSize;
        if(tempPtr->next == NULL)
            rightAddr = 1024;
        else
            rightAddr = tempPtr->next->memLocation;

        if(leftAddr < rightAddr) // If the two processes are not side by side
            if(n->memSize < (rightAddr - leftAddr)) { // If this hole is larger than what is needed, put the process in 
                n->memLocation = leftAddr;
                insertAfter(list, tempPtr, n); // Push this node after tempPtr in the list
                return true;
            }
        tempPtr = tempPtr->next;
    }
    return false; // No hole large enough

}
bool insertBest(node** list, node* n); // Attempts to move n into the smallest hole it can find that fits
bool insertWorst(node** list, node* n); // Attempts to move n into the largest hole it can find that fits
bool insertNext(node** list, node* n); // Attempts to move n into the next hole from the last placement

int calculateMemUsage(node* list) {
    float total = 0;

    if(list == NULL) {
        return 0;
    }

    node* tempPtr = list;
    while(tempPtr != NULL) {
        total += tempPtr->memSize;
        tempPtr = tempPtr->next;
    }
    return (total / (float)1024) * 100;
}
