/**
 * @file   check_mm.c
 * @Author Nicklas Bo Jensen (nboa@dtu.dk)
 * @date   September, 2013
 * @brief  Unit tests and suite for the memory management sub system.
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
 * Implements one suite of unit tests using the check 
 * unit testing framework.
 *
 * Add your new tests here. Feel free to organize 
 * your tests in multiple suites.
 *
 * The check framework includes a series of assert 
 * functions. See them all at: 
 * http://check.sourceforge.net/doc/check_html/check_4.html#Convenience-Test-Functions
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <check.h>
#include "mm.h"

/* Print debug messages for each alloc/free attempt */
#define VERBOSE_OUTPUT 0

/* Verify 32 byte alignment */
#define CHECK_ALIGNMENT 1


/** 
 * @name   Example simple allocation unit test 
 * @brief  Tests whether simple allocation works.
 */
START_TEST (test_simple_allocation) {
  int *ptr1;
  ptr1 = embedded_malloc(10 * sizeof(int));
  ck_assert(ptr1 != 0);
  
  embedded_free(ptr1);
}
END_TEST

/** 
 * @name   Example allocation overlap unit test.
 * @brief  Tests whether two allocations overlap.
 */
START_TEST (test_simple_unique_addresses) {
  int *ptr1;
  int *ptr2;
  ptr1 = embedded_malloc(10 * sizeof(int));
  ptr2 = embedded_malloc(10 * sizeof(int));
   
  /* Test whether each pointer have unique addresses*/
  ck_assert(ptr1 + 10 <= ptr2 || ptr2 + 10 <= ptr1);
  
  embedded_free(ptr1);
  embedded_free(ptr2);
}
END_TEST


/** 
 * @name   Example fragmentation unit test. 
 * @brief  Tests whether obvious fragmentation is avoided.
 */
START_TEST (test_simple_fragmentation) {
  int *ptr1;
  int *ptr2;

  ptr1 = embedded_malloc(10 * sizeof(int));
  ck_assert(ptr1 != 0);

  embedded_free(ptr1);

  ptr2 = embedded_malloc(10 * sizeof(int));
  ck_assert(ptr2 != 0);

  /* Test whether we avoid fragmentation in obvious case. */
  ck_assert(ptr2 <= ptr1);

  embedded_free(ptr2);
}
END_TEST

START_TEST (test_1) {
  int *ptr1;
  int i;
  for(i=0;i<100000;i++) {
  ptr1 = embedded_malloc(10 * sizeof(int));
  ck_assert(ptr1 != 0);
  embedded_free(ptr1);

  }

}
END_TEST

START_TEST (test_2) {
  int *ptr1;
  int *ptr2;
  int i;
  ptr1 = embedded_malloc(10 * sizeof(int));
  ck_assert(ptr1 != 0);
  
  for(i=0;i<10;i++){
    ptr1[i]=i+1;
  }
  
  for(i=0;i<100000;i++) {
  ptr2 = embedded_malloc(15 * sizeof(int));
  ck_assert(ptr2 !=0);
  embedded_free(ptr2);
  }
  for(i=1;i<=10;i++) { 
    ck_assert(ptr1[i-1]==i);
  }
}
END_TEST

START_TEST (test_3) {
  int *ptr1;
  int *ptr2;
  ptr1 = embedded_malloc(20 * sizeof(int));
  ptr2 = embedded_malloc(20 * sizeof(int));
  ck_assert(ptr1 != 0 && ptr2 != 0);
  ck_assert(ptr1 < ptr2);
  embedded_free(ptr1);
  ptr1 = embedded_malloc(20 * sizeof(int));
  ck_assert(ptr1 < ptr2);
  
}
END_TEST

START_TEST (test_4) {
  int *ptr1;
  int *ptr2;
  int *ptr3;
  ptr1 = embedded_malloc(20 * sizeof(int));
  ptr2 = embedded_malloc(20 * sizeof(int));
  ck_assert(ptr1 != 0 && ptr2 != 0);
  ck_assert(ptr1 < ptr2);
  embedded_free(ptr1);
  embedded_free(ptr2);
  ptr3=embedded_malloc(20 * sizeof(int));
  ck_assert(ptr3 < ptr2);
}
END_TEST

START_TEST (test_5) {
  int *ptr1;
  int *ptr2;
  int *ptr3;
  int *ptr4;
  ptr1 = embedded_malloc(20 * sizeof(int));
  ptr2 = embedded_malloc(20 * sizeof(int));
  ptr3 = embedded_malloc(20 * sizeof(int));

  ck_assert(ptr1 != 0 && ptr2 != 0);
  
  embedded_free(ptr2);
  embedded_free(ptr1);
  ptr4 = embedded_malloc(30 * sizeof(int));
  ck_assert(ptr4 < ptr3);
}
END_TEST

START_TEST (test_6) {
  int *ptr1;
  int *ptr2;
  int *ptr3;
  int *ptr4;
  ptr1 = embedded_malloc(20 * sizeof(int));
  ptr2 = embedded_malloc(20 * sizeof(int));
  ptr3 = embedded_malloc(20 * sizeof(int));

  ck_assert(ptr1 != 0 && ptr2 != 0);
  
  embedded_free(ptr1);
  embedded_free(ptr2);
  ptr4 = embedded_malloc(30 * sizeof(int));
  ck_assert(ptr4 < ptr3);
}
END_TEST

START_TEST (test_7) {
  
  int *ptr1;
  ptr1=embedded_malloc(784329398);
  ck_assert(ptr1==0);
}
END_TEST

START_TEST (test_8) {
  
  int *ptr1;
  ptr1=embedded_malloc(ALLOCATE_SIZE-BLOCKSIZE);
  ck_assert(ptr1!=0);
}
END_TEST

/* XOR a block of memory */
uint32_t sum_block(uint32_t *data, uint32_t size) {
  uint32_t sum = 0;
  int n;
  for (n=0; n < (size-31) >> 2; n++) {
    sum ^= data[n];
  }
  return sum;
}

/**
 * @name   Memory exerciser
 * @brief  Allocate and use memory blocks of varying sizes.
 * 
 *  When adding the test case to a test suite, remember to add:
 *
 *  tcase_set_timeout(tc_core, 0);
 *  tcase_add_test (tc_core, test_memory_exerciser);
 *
 *
 */
START_TEST (test_memory_exerciser) {
  uint32_t iterations = 1000; /* Alter as required */

  /* Struct to keep track of allocations */
  struct {
    void *addr;     /* Pointer returned by alloc */ 
    uint32_t *data; /* Pointer used for accessing region (will differ
		       from "addr" if alignment is handled by test) */
    uint32_t size;  /* Size of requested block */
    uint32_t crc;   /* Checksum of contents of block */
  } blocks[16];
  
  uint32_t  clock;
  uint32_t  total_memory_size=0;
  uint32_t  n;

  for(clock=0; clock<16; clock++) {
    blocks[clock].addr=0;
  }
  
  clock=0;
  
  while(iterations--) {
    char *addr;
    
    /* randomize the size of a block. */
    blocks[clock].size=(24*1024*1024-total_memory_size)*(rand()&(1024*1024-1))/
      (1024*8);
    
    /* Sanity check the block size. */
    if ((blocks[clock].size>0) && 
	(blocks[clock].size<(24*1024*1024))) {

      /* Try to allocate memory. */
      addr = embedded_malloc(blocks[clock].size);

      /* Check if it was successful. */
      ck_assert_msg(addr != NULL, "Memory allocation failed!");


#if CHECK_ALIGNMENT
      /* This address should already be 32 byte aligned */
      blocks[clock].data = (uint32_t *) addr;
#else
      /* When not implemented, we align it ourselves. */
      blocks[clock].data = (uint32_t *) ((uint32_t) (addr + 31) & (uint32_t) ~0x1f);
      ck_assert ((char *) blocks[clock].data >= addr);
#endif

      if ((uint32_t) blocks[clock].data & 0x1f) {
	printf("Unaligned address %xd (of %xd) returned!\n", blocks[clock].data, addr);
	ck_assert(0);
      }

#if VERBOSE_OUTPUT
	printf("alloc[%02d] %d bytes, total=%d\n", clock, blocks[clock].size, total_memory_size);
#endif

      /* Fill memory with data for verification */
      {
	uint32_t sum = 0;
	uint32_t x;
	for (n=0; n < (blocks[clock].size-31) >> 2; n++) {
	  x = rand();
	  blocks[clock].data[n] = x;
	  sum ^= x;
	}
	blocks[clock].crc = sum;
      }

      /* Keep track of how much memory we have allocated... */
      total_memory_size+=blocks[clock].size;

      /* and the address. */
      blocks[clock].addr=addr;
    }
    else {
      blocks[clock].addr=0;
    }

    /* Move on to next block */
    clock=(clock+1)&15;

    /* Verify all existing blocks before free */
    {
      uint32_t all_ok = 1;
      for (n=0; n < 16; n++) {
	if (blocks[n].addr != NULL) {
	  uint32_t sum = sum_block(blocks[n].data, blocks[n].size);

	  if (blocks[n].crc != sum) {
	    printf("Checksum failed for block %d at addr=%p: %08x != %08x\n", 
		   n, blocks[n].addr, blocks[n].crc, sum);
	    all_ok = 0;
	  }
	}
      }
      ck_assert_msg(all_ok, "Pre-free memory block corruption found\n");
    }

	 
    /* Try to free one block. */
    if (0 != blocks[clock].addr) {
#if VERBOSE_OUTPUT
      printf("free [%02d] %d bytes\n", clock, blocks[clock].size);
#endif

      embedded_free(blocks[clock].addr);
      total_memory_size-=blocks[clock].size;

      /* Mark block as free */
      blocks[clock].addr = NULL;

      /* Verify all existing blocks after free */
      {
	uint32_t all_ok = 1;
	for (n=0; n < 16; n++) {
	  if (blocks[n].addr != NULL) {
	    uint32_t sum = sum_block(blocks[n].data, blocks[n].size);

	    if (blocks[n].crc != sum) {
	      printf("Checksum failed for block %d at addr=%p: %08x != %08x\n", 
		     n, blocks[n].addr, blocks[n].crc, sum);
	      all_ok = 0;
	    }
	  }
	}
	ck_assert_msg(all_ok, "Post-free memory block corruption found\n");
      }
    }
  }

  /* Free final blocks */
  for (clock=0; clock < 16; clock++) {
    if (blocks[clock].addr != NULL) {
#if VERBOSE_OUTPUT
      printf("free [%02d] %d bytes\n", clock, blocks[clock].size);
#endif
      uint32_t sum = sum_block(blocks[clock].data, blocks[clock].size);

      if (blocks[clock].crc != sum) {
	printf("Checksum failed for block %d: %08x != %08x\n", clock, blocks[clock].crc, sum);
	ck_assert(0);
      }
      embedded_free(blocks[clock].addr);
    }
  }
}
END_TEST

/**
 * @name   Example unit test suite.
 * @brief  Add your new unit tests to this suite.
 *
 */
Suite* embedded_malloc_suite() {
  Suite *s = suite_create("embedded_malloc");
  TCase *tc_core = tcase_create("Core tests");
   tcase_add_test (tc_core, test_simple_allocation);
   tcase_add_test (tc_core, test_simple_unique_addresses);
   tcase_add_test (tc_core, test_simple_fragmentation);
   tcase_add_test (tc_core, test_1);
   tcase_add_test (tc_core, test_2);
   tcase_add_test (tc_core, test_3);
   tcase_add_test (tc_core, test_4);
   tcase_add_test (tc_core, test_5);
   tcase_add_test (tc_core, test_6);
   tcase_add_test (tc_core, test_7);
   tcase_add_test (tc_core, test_8);
   tcase_set_timeout(tc_core, 0);
   tcase_add_test (tc_core, test_memory_exerciser);


  suite_add_tcase(s, tc_core);
  return s;
}




/**
 * @name  Test runner
 * @bried This function runs the test suite and reports the result.
 *
 * If you organize your tests in multiple test suites, remember
 * to add the new suites to this function.
 */
int main() {
  int number_failed;
  Suite *s = embedded_malloc_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
