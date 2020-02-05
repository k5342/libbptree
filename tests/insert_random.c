#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	bptree_test_context *c = bptree_create_test_context(10, 200);
	
	insert_in_random(c);
	
	if (verify_tree(c) == BPTREE_TEST_FAILED){
		printf("verify_tree returned fail\n");
		goto fail;
	}
	bptree_destroy_context(c);
	return 0;
fail:
	bptree_destroy_context(c);
	return 1;
}
