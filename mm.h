/**
 * @file   mm.h
 * @Author Nicklas Bo Jensen (nboa@dtu.dk)
 * @date   September, 2013
 * @brief  Memory management skeleton header file.
 * 
 * Copyright 2013 Nicklas Bo Jensen, et. al., All rights reserved.
 *
 * You are allowed to change the file for the purpose of solving
 * exercises in the 02159 course at DTU. You are not allowed to
 * change this message or disclose this file to anyone except for
 * course staff in the 02159 course at DTU. Disclosing contents
 * to fellow course mates or redistributing contents is a direct
 * violation.
*/

#include <stddef.h>

typedef struct block * blockPtr;

 struct block {
  int full;
  size_t size;
  blockPtr prev;
  blockPtr next;
};


/**
 * @name    ALLOCATE_SIZE
 * @brief   Amount of memory available for allocation, 32MB.
 *
 */
#define ALLOCATE_SIZE    32*1024*1024
#define BLOCKSIZE 16

/**
 * @name    embedded_malloc
 * @brief   Allocate at least size contiguous bytes of memory and return a pointer to the first byte.
 */
void* embedded_malloc(size_t size);


/**
 * @name    embedded_free
 * @brief   Frees previously allocated memory and make it available for subsequent calls to embedded_malloc.
 */
void embedded_free(void *ptr);


void printMemory(int i);
