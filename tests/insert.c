#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	printf("libptree test: insert\n");
	
	int is_failed = 0;
	is_failed |= do_test("insert in ascending order", insert_in_asc, check_search);
	is_failed |= do_test("insert in descending order", insert_in_desc, check_search);
	is_failed |= do_test("insert in random order", insert_in_random, check_search);
	
	printf("Overall Result: ");
	if (is_failed){
		printf("FAILED\n");
	} else {
		printf("PASSED\n");
	}
	return is_failed;
}
