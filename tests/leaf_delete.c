#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_check_leaf_delete_single(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)0, (void *)0x0);
	if (bptree_leaf_get_key_count(bpt, leaf) != 1){
		goto fail;
	}
	bptree_leaf_delete(bpt, leaf, (bptree_key_t)0);
	if (bptree_leaf_get_key_count(bpt, leaf) != 0){
		goto fail;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_leaf_delete_first_from_two(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)0, (void *)0x0);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)1, (void *)0x1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 2){
		printf("Unexpected key count after insert (maybe insert is broken)\n");
		goto fail;
	}
	bptree_leaf_delete(bpt, leaf, (bptree_key_t)0);
	if (bptree_leaf_get_key_count(bpt, leaf) != 1){
		printf("Unexpected key count after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 0) != (bptree_key_t)1){
		printf("Unexpected key value at index = 0 after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 0) != (void *)0x1){
		printf("Unexpected element value at index = 0 after delete\n");
		goto fail;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_leaf_delete_last_from_two(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)0, (void *)0x0);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)1, (void *)0x1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 2){
		printf("Unexpected key count after insert (maybe insert is broken)\n");
		goto fail;
	}
	bptree_leaf_delete(bpt, leaf, (bptree_key_t)1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 1){
		printf("Unexpected key count after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 0) != (bptree_key_t)0){
		printf("Unexpected key value at index = 0 after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 0) != (void *)0x0){
		printf("Unexpected element value at index = 0 after delete\n");
		goto fail;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_leaf_delete_middle_from_three(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)0, (void *)0x0);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)1, (void *)0x1);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)2, (void *)0x2);
	if (bptree_leaf_get_key_count(bpt, leaf) != 3){
		printf("Unexpected key count after insert (maybe insert is broken)\n");
		goto fail;
	}
	bptree_leaf_delete(bpt, leaf, (bptree_key_t)1);
	if (bptree_leaf_get_key_count(bpt, leaf) != 2){
		printf("Unexpected key count after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 0) != (bptree_key_t)0){
		printf("Unexpected key value at index = 0 after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 0) != (void *)0x0){
		printf("Unexpected element value at index = 0 after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 1) != (bptree_key_t)2){
		printf("Unexpected key value at index = 1 after delete\n");
		goto fail;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 1) != (void *)0x2){
		printf("Unexpected element value at index = 1 after delete\n");
		goto fail;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_leaf_delete_right_and_merge(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	for (int i = 0; i < 11; i++){
		bptree_leaf_insert(bpt, leaf, (bptree_key_t)i, (void *)i);
	}
	
	bptree_node_t *left_leaf = leaf;
	bptree_node_t *right_leaf = bptree_leaf_get_rightadjacent(bpt, leaf);
	
	bptree_leaf_delete(bpt, right_leaf, (bptree_key_t)6);
	
	int expected[] = {0, 1, 2, 3, 4, 5, 7, 8, 9, 10};
	for (int i = 0; i < 10; i++){
		if (bptree_leaf_get_key_by_index(bpt, left_leaf, i) != (bptree_key_t)expected[i]){
			printf("Unexpected key value at index = %d on right_leaf\n", i);
			goto fail;
		}
		if (bptree_leaf_get_element_by_index(bpt, left_leaf, i) != (void *)expected[i]){
			printf("Unexpected element value at index = %d on right_leaf\n", i);
			goto fail;
		}
	}
	
	// after delete, two leaves must be merged into a single leaf
	if (bptree_node_is_leaf(bpt, bpt->root) != 1){
		printf("root node must be a leaf\n");
		goto fail;
	}
	if (bptree_leaf_get_rightadjacent(bpt, bpt->root) != NULL){
		printf("The adjacent leaf of left_leaf should be NULL\n");
		goto fail;
	}
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_leaf_delete_left_and_merge(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	for (int i = 0; i < 11; i++){
		bptree_leaf_insert(bpt, leaf, (bptree_key_t)i, (void *)i);
	}
	
	bptree_node_t *left_leaf = leaf;
	
	bptree_leaf_delete(bpt, left_leaf, (bptree_key_t)0);
	bptree_leaf_delete(bpt, left_leaf, (bptree_key_t)1);
	
	int expected[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
	for (int i = 0; i < 9; i++){
		if (bptree_leaf_get_key_by_index(bpt, left_leaf, i) != (bptree_key_t)expected[i]){
			printf("Unexpected key value at index = %d on left_leaf\n", i);
			goto fail;
		}
		if (bptree_leaf_get_element_by_index(bpt, left_leaf, i) != (void *)expected[i]){
			printf("Unexpected element value at index = %d on left_leaf\n", i);
			goto fail;
		}
	}
	
	// after delete, two leaves must be merged into a single leaf
	if (bptree_node_is_leaf(bpt, bpt->root) != 1){
		printf("root node must be a leaf\n");
		goto fail;
	}
	if (bptree_leaf_get_rightadjacent(bpt, bpt->root) != NULL){
		printf("The adjacent leaf of left_leaf should be NULL\n");
		goto fail;
	}
	
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

int main(int argc, char *argv[]){
	do_single_test("check delete a single value from a leaf", testcase_check_leaf_delete_single);
	do_single_test("check delete first value from two elements of a leaf", testcase_check_leaf_delete_first_from_two);
	do_single_test("check delete last value from two elements of a leaf", testcase_check_leaf_delete_last_from_two);
	do_single_test("check delete middle value from a leaf", testcase_check_leaf_delete_middle_from_three);
	do_single_test("check delete from left_leaf and then merge from two leaves", testcase_check_leaf_delete_left_and_merge);
	do_single_test("check delete from right_leaf and then merge from two leaves", testcase_check_leaf_delete_right_and_merge);
}
