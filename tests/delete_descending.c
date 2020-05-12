#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

int main(int argc, char *argv[]){
	bptree_test_context *c = bptree_create_test_context(10, 200);
	
	insert_in_desc(c);
	delete_in_asc(c);
	
	bptree_destroy_test_context(c);
	return 0;
}
