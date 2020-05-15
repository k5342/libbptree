#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	bptree_test_context *c = bptree_create_test_context(2, 200);
	
	insert_in_random(c);
	if (delete_in_asc(c) != BPTREE_TEST_PASSED){
		printf("delete test fail\n");
		goto fail;
	}
	
	bptree_destroy_test_context(c);
	return 0;
fail:
	bptree_destroy_test_context(c);
	return 1;
}
