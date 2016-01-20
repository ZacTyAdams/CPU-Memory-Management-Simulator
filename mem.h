#ifndef MEMHEADER
#define MEMHEADER

using namespace std;
#include <iostream>
#include <stdio.h>

void initMemPool(char*, long int);
void *my_malloc(int elem_size);
void my_free(void *p);

#endif
