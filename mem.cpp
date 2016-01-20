using namespace std;
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "mem.h"

//pointers to point to beginning and end of memory pool
char* memStartPtr;
char* memEndPtr;

//variables to keep track of memory statistics
long int maxMemory;
int memUsed;
int blockCount;

//structure to indicate 
typedef struct{
		int sizeCount;
		int isOpen;
}memBlock, *memBlockPtr;

//initialize memory stats and pointers 
void initMemPool(char* frontPtr, long int poolSize){
	maxMemory = poolSize;
	memUsed = 0;
	memStartPtr = frontPtr;
	memEndPtr = frontPtr + poolSize;
    //memset(memStartPtr,0x00,poolSize);
	cout << "Memory pool initialized to " << maxMemory << " bytes." << endl;
}

//custom malloc function to allocate appropriate structures and pointers across
//memory span initialized
void *my_malloc(int requestedSize){
    /* check whether any chunk (allocated before) is free first */
    memBlockPtr blockPtr;
    int flag = 1;
 
    blockPtr = (memBlockPtr)memStartPtr;
 
    int overhead = sizeof(memBlock);
 
    if((requestedSize + overhead)  > (maxMemory - (memUsed + blockCount * overhead))){
        //exceeded max size, return null ptr
        return NULL;
    }
    while(memEndPtr > ((char *)blockPtr + requestedSize + overhead)){
        if(blockPtr->isOpen == 0){
            if(blockPtr->sizeCount == 0){
                flag = 0;
                break;
            }
            if(blockPtr->sizeCount > (requestedSize + overhead)){
                flag = 2;
                break;
            }
        }
        blockPtr = (memBlockPtr)((char *)blockPtr + blockPtr->sizeCount);
    }
    if(flag != 1){
        blockPtr->isOpen = 1;
        if(flag == 0){
            blockPtr->sizeCount = requestedSize + sizeof(memBlock);
        }
        else if(flag == 2){
            requestedSize = blockPtr->sizeCount - sizeof(memBlock);
        }
        blockCount++;
        memUsed += requestedSize;
        return ((char *)blockPtr + overhead);
    }
    //no space was found within the memory bounds, return null ptr
    return NULL;
}

void my_free(void *item){
    /* Mark in MCB that this chunk is free */
    memBlockPtr blockPtr = (memBlockPtr)item;
    blockPtr--;
    blockCount--;
    blockPtr->isOpen = 0;
    memUsed -= (blockPtr->sizeCount - sizeof(memBlock));  
    //printf("\nAllocated mem: %d ",blockPtr->sizeCount);
    //printf("\nAllocated mem: %d ",memUsed);
    //printf("\nMemory Freed...\n");
}