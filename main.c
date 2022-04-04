// Christian Catalano, Assignment 2, March 19th, 1120832
#include "main.h"

int main(int argc, char* argv[]) {
    algo algoType;

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

    /* LINKED LIST TESTING
    push(&list, 1,2,3);
    printList(list);
    node* head = pop(&list);
    push(&list, 3,4,5);
    printList(head);
    printList(list);
    freeList(list);
    free(head);*/


    /* Print Stats */

    printList(waitingList);
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

node* pop(node** list) {
    node* head = *list;
    *list = head->next;
    return head;
}

node* createNode(int pid, int memSize, int memLocation) {
    node* newNode = malloc(sizeof(node));
    newNode->pid = pid;
    newNode->memSize = memSize;
    newNode->memLocation = memLocation;
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