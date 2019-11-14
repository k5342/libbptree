#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	printf("libptree test: insert\n");
	
	int is_failed = 0;
	if (do_test("insert in ascending order",
				insert_in_asc, verify_tree) == BPTREE_TEST_FAILED){
		is_failed |= 1;
	}
	if (do_test("insert in descending order",
				insert_in_desc, verify_tree) == BPTREE_TEST_FAILED){
		is_failed |= 1;
	}
	if (do_test("insert in random order",
				insert_in_random, verify_tree) == BPTREE_TEST_FAILED){
		is_failed |= 1;
	}
	
	printf("Overall Result: ");
	if (is_failed){
		printf("FAILED\n");
		return 1;
	} else {
		printf("PASSED\n");
		return 0;
	}
}
