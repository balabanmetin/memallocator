/**
 * @file   mm.c
 * @Author Nicklas Bo Jensen (nboa@dtu.dk)
 * @date   September, 2013
 * @brief  Memory management skeleton.
 * 
 * Copyright 2013 Nicklas Bo Jensen, et. al., All rights reserved.
 *
 * You are allowed to change the file for the purpose of solving
 * exercises in the 02159 course at DTU. You are not allowed to
 * change this message or disclose this file to anyone except for
 * course staff in the 02159 course at DTU. Disclosing contents
 * to fellow course mates or redistributing contents is a direct
 * violation.
 *
 *
 *
 * Replace the functions in this file with your own implementations.
 * You should not modify the function prototypes. 
 * 
 * As part of the assignment add new unit tests in 
 * check_mm.c.
 *
 */

#include <stdint.h>
#include "mm.h"


/**
 * @name    Memory array
 * @brief   Array representing the available memory.
 *
 * Actual memory blob. You should allocate and manage memory in this array.
 */
static int8_t memory[ALLOCATE_SIZE];
static blockPtr base=0;
static blockPtr last;




char isValid(char* ptr) { 
  blockPtr block= (blockPtr)(ptr-BLOCKSIZE);
  if( block->full==1 && 
      (block->next==0 || (block->next >= base && block->next <= (blockPtr)(((char*)base)+ALLOCATE_SIZE))) &&
      (block->prev==0 || (block->prev >= base && block->prev <= (blockPtr)(((char*)base)+ALLOCATE_SIZE))) &&
      block->size<=ALLOCATE_SIZE )
    return 1;
  return 0;
}

blockPtr merge(blockPtr first, blockPtr second) { 
  first->next=second->next;
  first->size=((char*)second)- ((char*)first) + second->size;
  if(second->next)
    second->prev=first;
  return first;
}



blockPtr findPlace(size_t size) {
  blockPtr _base=base;
  last=0;

  while(_base && (_base->full ||  _base->size < size)) {
    last=_base;
    _base=_base->next;
  }
  return _base;
}

blockPtr extendMemory(size_t size) {
  blockPtr new=0;
  if(last!=0) {
    new=(blockPtr)((char*)last + BLOCKSIZE + last->size);
    last->next=new;
  }
  else {
    new=(blockPtr)memory;
    base=new;
  }
  new->prev=last;/* if last is null then it holds. if last isn't full it still holds*/
  new->size = size;
  new->next=0;
  return new;
}

blockPtr splitMemory(blockPtr current ,size_t size) {
  blockPtr smallPart;
  
  if(current->size >= size + BLOCKSIZE + 4) {
    smallPart=(blockPtr)((char*)current + BLOCKSIZE + size);
    smallPart->next=current->next;
    smallPart->next->prev=smallPart;
    current->next=smallPart;
    smallPart->prev=current;
    smallPart->full=0;
    smallPart->size=current->size - BLOCKSIZE - size;
    current->size = size;
    return current;
  }
  else {

    /* ?? */
    return current;
  }
}


/**
 * @name    embedded_free
 * @brief   Frees previously allocated memory and make it available for subsequent calls to embedded_malloc
 *
 * This function should behave similar to a normal free implementation. 
 *
 * @param void *ptr Pointer to the memory to free.
 *
 */

 void embedded_free(void * ptr) {

  blockPtr freed,thePrev,theNext,temp;
  if(!isValid((char*)ptr)) {
    return;
  }

  freed=(blockPtr)(((char*)ptr)-BLOCKSIZE);
  theNext= freed->next;
  thePrev= freed->prev;

  if(theNext && !theNext->full){
    temp=merge(freed, theNext);
    temp->full=1;
    embedded_free(((char*)temp)+BLOCKSIZE);
  }
  else if(theNext && theNext->full){
    if(thePrev && !thePrev->full){
      temp=merge(thePrev,freed);
      theNext->prev=temp;
    }
    else {
      freed->full=0;
    }
  }
  else {
    if(!thePrev) {
      base=0;
    }
    else {
      thePrev->next=0;
      if(!thePrev->full) {
	thePrev->full=1;
	embedded_free(((char*)thePrev)+BLOCKSIZE);
      }
    }
  }

}

/**
 * @name    embedded_malloc
 * @brief   Allocate at least size contiguous bytes of memory and return a pointer to the first byte.
 *
 * This function should behave similar to a normal malloc implementation. 
 *
 * @param size_t size Number of bytes to allocate.
 * @retval Pointer to the start of the allocated memory.
 *
 */


void* embedded_malloc(size_t _size) {
  
  register int t= (_size & 0x1f);
  size_t size = t==0 ? _size : _size + 32 - t;
  blockPtr p = findPlace(size), found;
  if(p == 0) { /*checking if it fits */
    register int a = ALLOCATE_SIZE - (last-base)-BLOCKSIZE;
    if(last!=0)
      a-=last->size+BLOCKSIZE;
    if(size > a)
      return 0;
   found= extendMemory(size); 
  }
  else {
    found = splitMemory(p,size);
  }
  found->full=1;
  return (void*)((char *)found + BLOCKSIZE);

}
