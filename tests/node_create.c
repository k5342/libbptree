#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_check_passed_null(){
	bptree_node_t *node = bptree_node_create(NULL);
	if (node == NULL){
		return BPTREE_TEST_PASSED;
	}
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_passed_not_null(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	if (node == NULL){
		bptree_node_destroy(node);
		bptree_free(bpt);
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_context(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	if (node->context != bpt){
		bptree_node_destroy(node);
		bptree_free(bpt);
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_parent(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	if (node->parent != NULL){
		bptree_node_destroy(node);
		bptree_free(bpt);
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_children(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	for (int i = 0; i < bpt->nkeys + 1; i++){
		node->children[i] = (bptree_node_t *)i;
	}
	for (int i = 0; i < bpt->nkeys + 1; i++){
		if (node->children[i] != (bptree_node_t *)i){
			bptree_node_destroy(node);
			bptree_free(bpt);
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_keys(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	for (int i = 0; i < bpt->nkeys; i++){
		node->keys[i] = (bptree_key_t)i;
	}
	for (int i = 0; i < bpt->nkeys; i++){
		if (node->keys[i] != (bptree_key_t)i){
			bptree_node_destroy(node);
			bptree_free(bpt);
			return BPTREE_TEST_FAILED;
		}
	}
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_used(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	if (node->used != 0){
		bptree_node_destroy(node);
		bptree_free(bpt);
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_check_is_not_leaf(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	if (node->is_leaf != 0){
		bptree_node_destroy(node);
		bptree_free(bpt);
		return BPTREE_TEST_FAILED;
	}
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
}

int main(int argc, char *argv[]){
	do_single_test("check bptree_node_creste(NULL) returns NULL", testcase_check_passed_null);
	do_single_test("check bptree_node_create(bpt) returns not NULL", testcase_check_passed_not_null);
	do_single_test("check node->context == NULL", testcase_check_context);
	do_single_test("check node->parent == NULL", testcase_check_parent);
	do_single_test("check node->children is valid", testcase_check_children);
	do_single_test("check node->keys is valid", testcase_check_keys);
	do_single_test("check node->used == 0", testcase_check_used);
	do_single_test("check node->is_leaf != 0", testcase_check_is_not_leaf);
}
