#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_check_passed_null(){
	bptree_node_t *leaf = bptree_leaf_create(NULL);
	if (leaf == NULL){
		return BPTREE_TEST_PASSED;
	}
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_passed_not_null(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	if (leaf != NULL){
		return BPTREE_TEST_PASSED;
	}
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_context(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	if (leaf->context != bpt){
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_parent(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	if (leaf->parent != NULL){
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_children(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	for (int i = 0; i < bpt->nkeys + 1; i++){
		leaf->children[i] = (bptree_node_t *)i;
	}
	for (int i = 0; i < bpt->nkeys + 1; i++){
		if (leaf->children[i] != (bptree_node_t *)i){
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_keys(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	for (int i = 0; i < bpt->nkeys; i++){
		leaf->keys[i] = (bptree_key_t)i;
	}
	for (int i = 0; i < bpt->nkeys; i++){
		if (leaf->keys[i] != (bptree_key_t)i){
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_used(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	if (leaf->used != 0){
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_is_leaf(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	if (leaf->is_leaf != 1){
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_right_most_child(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bptree_leaf_create(bpt);
	if (leaf->children[bpt->nkeys] != NULL){
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(leaf);
	return BPTREE_TEST_PASSED;
}

int main(int argc, char *argv[]){
	do_single_test("check bptree_leaf_creste(NULL) returns NULL", testcase_check_passed_null);
	do_single_test("check bptree_leaf_create(bpt) returns not NULL", testcase_check_passed_not_null);
	do_single_test("check leaf->context == NULL", testcase_check_context);
	do_single_test("check leaf->parent == NULL", testcase_check_parent);
	do_single_test("check leaf->children is valid", testcase_check_children);
	do_single_test("check leaf->keys is valid", testcase_check_keys);
	do_single_test("check leaf->used == 0", testcase_check_used);
	do_single_test("check leaf->is_leaf != 1", testcase_check_is_leaf);
}
