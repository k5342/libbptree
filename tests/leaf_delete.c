#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_check_leaf_delete_single(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, 0, 0x0);
	if (bptree_leaf_get_key_count(bpt, leaf) != 1){
		return BPTREE_TEST_FAILED;
	}
	bptree_leaf_delete(bpt, leaf, 0);
	if (bptree_leaf_get_key_count(bpt, leaf) != 0){
		return BPTREE_TEST_FAILED;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_leaf_delete_first_from_two(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, 0, 0x0);
	bptree_leaf_insert(bpt, leaf, 1, 0x1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 2){
		printf("Unexpected key count after insert (maybe insert is broken)\n");
		return BPTREE_TEST_FAILED;
	}
	bptree_leaf_delete(bpt, leaf, 0);
	if (bptree_leaf_get_key_count(bpt, leaf) != 1){
		printf("Unexpected key count after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 0) != (bptree_key_t)1){
		printf("Unexpected key value at index = 0 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 0) != 0x1){
		printf("Unexpected element value at index = 0 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_leaf_delete_last_from_two(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, 0, 0x0);
	bptree_leaf_insert(bpt, leaf, 1, 0x1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 2){
		printf("Unexpected key count after insert (maybe insert is broken)\n");
		return BPTREE_TEST_FAILED;
	}
	bptree_leaf_delete(bpt, leaf, 1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 1){
		printf("Unexpected key count after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 0) != (bptree_key_t)0){
		printf("Unexpected key value at index = 0 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 0) != 0x0){
		printf("Unexpected element value at index = 0 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_leaf_delete_middle_from_three(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, 0, 0x0);
	bptree_leaf_insert(bpt, leaf, 1, 0x1);
	bptree_leaf_insert(bpt, leaf, 2, 0x2);
	if (bptree_leaf_get_key_count(bpt, leaf) != 3){
		printf("Unexpected key count after insert (maybe insert is broken)\n");
		return BPTREE_TEST_FAILED;
	}
	bptree_leaf_delete(bpt, leaf, 1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 2){
		printf("Unexpected key count after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 0) != (bptree_key_t)0){
		printf("Unexpected key value at index = 0 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 0) != 0x0){
		printf("Unexpected element value at index = 0 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 1) != (bptree_key_t)2){
		printf("Unexpected key value at index = 1 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 1) != 0x2){
		printf("Unexpected element value at index = 1 after delete\n");
		return BPTREE_TEST_FAILED;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

int main(int argc, char *argv[]){
	do_single_test("check delete a single value from a leaf", testcase_check_leaf_delete_single);
	do_single_test("check delete first value from two elements of a leaf", testcase_check_leaf_delete_first_from_two);
	do_single_test("check delete last value from two elements of a leaf", testcase_check_leaf_delete_last_from_two);
	do_single_test("check delete middle value from a leaf", testcase_check_leaf_delete_middle_from_three);
}
