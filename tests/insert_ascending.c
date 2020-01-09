#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	bptree_test_context *c = bptree_create_test_context(10, 200);
	
	insert_in_asc(c);
	
	if (verify_tree(c) == BPTREE_TEST_FAILED){
		printf("verify_tree returned fail");
		return 1;
	}
	if (check_search(c) == BPTREE_TEST_FAILED){
		printf("check_search returned fail");
		return 1;
	}
	if (check_min_or_max(c) == BPTREE_TEST_FAILED){
		printf("check_min_or_max returned fail");
		return 1;
	}
	if (check_leaf(c) == BPTREE_TEST_FAILED){
		printf("check_leaf returned fail");
		return 1;
	}
}
