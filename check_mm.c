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

#include <check.h>
#include "mm.h"

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
