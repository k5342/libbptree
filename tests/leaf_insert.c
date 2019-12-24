#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_check_leaf_insert_single(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	// insert a single (k, v) then check leaf
	bptree_leaf_insert(bpt, leaf, 0, 0x0);
	if (leaf->used != 1){
		return BPTREE_TEST_FAILED;
	}
	if (leaf->keys[leaf->used - 1] != 0){
		return BPTREE_TEST_FAILED;
	}
	if (leaf->children[leaf->used - 1] != 0x0){
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_leaf_insert_no_divide(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	for (int i = 0; i < 10; i++){
		bptree_leaf_insert(bpt, leaf, (bptree_key_t)i, (void *)i);
	}
	if (leaf->used != 10){
		return BPTREE_TEST_FAILED;
	}
	for (int i = 0; i < 10; i++){
		if (leaf->keys[i] != (bptree_key_t)i){
			return BPTREE_TEST_FAILED;
		}
		if (leaf->children[i] != (void *)i){
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_leaf_insert_divide_even(){
	bptree_t *bpt = bptree_init(10);
	for (int i = 0; i < 11; i++){
		bptree_leaf_insert(bpt, bpt->root, (bptree_key_t)i, (void *)i);
	}
	
	// check divided root
	if (bpt->root->used != 1){
		printf("bpt->root->used != 1 (expected 1, but %d)\n", bpt->root->used);
		bptree_print(bpt);
		return BPTREE_TEST_FAILED;
	}
	if (bpt->root->keys[0] != (bptree_key_t)6){
		printf("bpt->root->keys[0] != 6 (expected 6, but %lld)\n", bpt->root->keys[0]);
		return BPTREE_TEST_FAILED;
	}

	// check children
	bptree_node_t *left_child = bpt->root->children[0];
	bptree_node_t *right_child = bpt->root->children[1];
	
	if (left_child->used != 6){
		printf("left_child->used != 6 (expected 6, but %d)\n", left_child->used);
		return BPTREE_TEST_FAILED;
	}
	if (right_child->used != 5){
		printf("left_child->used != 5 (expected 5, but %d)\n", right_child->used);
		return BPTREE_TEST_FAILED;
	}
	
	for (int i = 0; i < 6; i++){
		if (left_child->keys[i] != (bptree_key_t)i){
			printf("left_child->keys[i] != i (expected %d, but %d)\n", i, left_child->keys[i]);
			return BPTREE_TEST_FAILED;
		}
		if (left_child->children[i] != (void *)i){
			printf("left_child->children[i] != i (expected %p, but %p)\n", (void *)i, left_child->children[i]);
			return BPTREE_TEST_FAILED;
		}
	}
	
	for (int i = 0; i < 5; i++){
		if (right_child->keys[i] != (bptree_key_t)(i + 6)){
			printf("right_child->keys[i] != i (expected %d, but %d)\n", (i + 6), right_child->keys[i]);
			return BPTREE_TEST_FAILED;
		}
		if (right_child->children[i] != (void *)(i + 6)){
			printf("right_child->children[i] != i (expected %p, but %p)\n", (void *)(i + 6), right_child->children[i]);
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_node_destroy(bpt->root);
	bptree_node_destroy(left_child);
	bptree_node_destroy(right_child);
	return BPTREE_TEST_PASSED;
}

int main(int argc, char *argv[]){
	do_single_test("check insert a single key-value", testcase_check_leaf_insert_single);
	do_single_test("check insert within a capacity of root", testcase_check_leaf_insert_no_divide);
	do_single_test("check insert in (capacity + 1)", testcase_check_leaf_insert_divide_even);
}
