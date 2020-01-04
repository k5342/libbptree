#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	bptree_test_context *c = bptree_create_test_context(11);
	
	insert_in_asc(c);
	
	if (verify_tree(c) == BPTREE_TEST_FAILED){
		printf("verify_tree returned fail");
		return 1;
	}
}
