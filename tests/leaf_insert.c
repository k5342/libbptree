#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_check_leaf_insert_single(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	// insert a single (k, v) then check leaf
	bptree_leaf_insert(bpt, leaf, 0, 0x0);
	if (bptree_leaf_get_key_count(bpt, leaf) != 1){
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, 0) != (bptree_key_t)0){
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, 0) != (void *)0x0){
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_key_by_index(bpt, leaf, -1) != (bptree_key_t)0){
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_element_by_index(bpt, leaf, -1) != (void *)0x0){
		return BPTREE_TEST_FAILED;
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_leaf_insert_no_divide(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	for (int i = 0; i < 10; i++){
		bptree_leaf_insert(bpt, leaf, (bptree_key_t)i, (void *)i);
	}
	if (bptree_leaf_get_key_count(bpt, leaf) != 10){
		return BPTREE_TEST_FAILED;
	}
	for (int i = 0; i < 10; i++){
		if (bptree_leaf_get_key_by_index(bpt, leaf, i) != (bptree_key_t)i){
			return BPTREE_TEST_FAILED;
		}
		if (bptree_leaf_get_element_by_index(bpt, leaf, i) != (void *)i){
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_leaf_insert_divide_even(){
	bptree_t *bpt = bptree_init(10);
	for (int i = 0; i < 11; i++){
		bptree_leaf_insert(bpt, bpt->root, (bptree_key_t)i, (void *)i);
	}
	
	// check divided root
	if (bptree_leaf_get_key_count(bpt, bpt->root) != 1){
		printf("bpt->root->used != 1 (expected 1, but %d)\n", bptree_leaf_get_key_count(bpt, bpt->root));
		bptree_print(bpt);
		return BPTREE_TEST_FAILED;
	}
	if (bptree_leaf_get_key_by_index(bpt, bpt->root, 0) != (bptree_key_t)6){
		printf("bpt->root->keys[0] != 6 (expected 6, but %lld)\n", bptree_leaf_get_key_by_index(bpt, bpt->root, 0));
		return BPTREE_TEST_FAILED;
	}

	// check children
	bptree_node_t *left_child = bptree_node_get_children_by_index(bpt, bpt->root, 0);
	bptree_node_t *right_child = bptree_node_get_children_by_index(bpt, bpt->root, 1);
	
	if (bptree_node_get_key_count(bpt, left_child) != 6){
		printf("left_child->used != 6 (expected 6, but %d)\n", bptree_node_get_key_count(bpt, left_child));
		return BPTREE_TEST_FAILED;
	}
	if (bptree_node_get_key_count(bpt, right_child) != 5){
		printf("right_child->used != 5 (expected 5, but %d)\n", bptree_node_get_key_count(bpt, right_child));
		return BPTREE_TEST_FAILED;
	}
	
	for (int i = 0; i < 6; i++){
		if (bptree_node_get_key_by_index(bpt, left_child, i) != (bptree_key_t)i){
			printf("left_child->keys[i] != i (expected %d, but %lld)\n", i, bptree_node_get_key_by_index(bpt, left_child, i));
			return BPTREE_TEST_FAILED;
		}
		if (bptree_node_get_children_by_index(bpt, left_child, i) != (void *)i){
			printf("left_child->children[i] != i (expected %p, but %p)\n", (void *)i, bptree_node_get_children_by_index(bpt, left_child, i));
			return BPTREE_TEST_FAILED;
		}
	}
	
	for (int i = 0; i < 5; i++){
		if (bptree_node_get_key_by_index(bpt, right_child, i) != (bptree_key_t)(i + 6)){
			printf("right_child->keys[i] != i (expected %d, but %lld)\n", (i + 6), bptree_node_get_key_by_index(bpt, right_child, i));
			return BPTREE_TEST_FAILED;
		}
		if (bptree_node_get_children_by_index(bpt, right_child, i) != (void *)(i + 6)){
			printf("right_child->children[i] != i (expected %p, but %p)\n", (void *)(i + 6), bptree_node_get_children_by_index(bpt, right_child, i));
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

int main(int argc, char *argv[]){
	do_single_test("check insert a single key-value", testcase_check_leaf_insert_single);
	do_single_test("check insert within a capacity of root", testcase_check_leaf_insert_no_divide);
	do_single_test("check insert in (capacity + 1)", testcase_check_leaf_insert_divide_even);
}
