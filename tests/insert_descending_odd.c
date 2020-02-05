#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	bptree_test_context *c = bptree_create_test_context(11, 200);
	
	insert_in_desc(c);
	
	if (verify_tree(c) == BPTREE_TEST_FAILED){
		printf("verify_tree returned fail\n");
		bptree_destroy_context(c);
		return 1;
	}
	bptree_destroy_context(c);
	return 0;
}
