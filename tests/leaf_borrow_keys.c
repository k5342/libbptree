#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

// create
// [
//   {0x0 0 0x1 1 0x2 2 0x3 3 0x4 4 0x5 5|XXXX}
//   10
//   {0xa 10 0xb 11 0xc 12 0xd 13 0xe 14 0xf 15|XXXX}
//   20
//   {0x14 20 0x15 21 0x16 22 0x17 23 0x18 24 0x19 25|(nil)}
// ]
void setup_environment(bptree_t *bpt){
	for (int i = 0; i < 6; i++){
		bptree_insert(bpt, (bptree_key_t)i, (void *)i);
	}
	for (int i = 10; i < 16; i++){
		bptree_insert(bpt, (bptree_key_t)i, (void *)i);
	}
	for (int i = 20; i < 26; i++){
		bptree_insert(bpt, (bptree_key_t)i, (void *)i);
	}
}

bptree_test_result testcase_check_leaf_borrow_keys_from_left(){
	bptree_t *bpt = bptree_init(10);
	setup_environment(bpt);
	
	bptree_node_t *left_leaf, *right_leaf, *last_leaf;
	left_leaf = bpt->root->children[0];
	right_leaf = bpt->root->children[1];
	last_leaf = bpt->root->children[2];
	
	bptree_leaf_borrow_keys(bpt, left_leaf, right_leaf, right_leaf, 2, 0);
	bptree_node_t *expected_root_children[] = {left_leaf, right_leaf, last_leaf};
	int expected_root_keys[] = {4, 20};
	for(int i = 0; i < 2; i++){
		if (expected_root_keys[i] != bpt->root->keys[i]){
			printf("Error: bpt->root->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_root_keys[i], bpt->root->keys[i]);
			goto fail;
		}
	}
	for(int i = 0; i < 3; i++){
		if (expected_root_children[i] != bpt->root->children[i]){
			printf("Error: bpt->root->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_root_children[i], bpt->root->children[i]);
			goto fail;
		}
	}
	
	int expected_left_leaf_keys[] = {0, 1, 2, 3};
	for(int i = 0; i < 4; i++){
		if (expected_left_leaf_keys[i] != left_leaf->keys[i]){
			printf("Error: left_leaf->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_left_leaf_keys[i], left_leaf->keys[i]);
			goto fail;
		}
		if (expected_left_leaf_keys[i] != left_leaf->children[i]){
			printf("Error: left_leaf->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, (void *)expected_left_leaf_keys[i], left_leaf->children[i]);
			goto fail;
		}
	}
	
	int expected_right_leaf_keys[] = {4, 5, 10, 11, 12, 13, 14, 15};
	for(int i = 0; i < 8; i++){
		if (expected_right_leaf_keys[i] != right_leaf->keys[i]){
			printf("Error: right_leaf->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_right_leaf_keys[i], right_leaf->keys[i]);
			goto fail;
		}
		if (expected_right_leaf_keys[i] != right_leaf->children[i]){
			printf("Error: right_leaf->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, (void *)expected_right_leaf_keys[i], right_leaf->children[i]);
			goto fail;
		}
	}
	
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_leaf_borrow_keys_from_right(){
	bptree_t *bpt = bptree_init(10);
	setup_environment(bpt);
	
	bptree_node_t *left_leaf, *right_leaf, *last_leaf;
	left_leaf = bpt->root->children[0];
	right_leaf = bpt->root->children[1];
	last_leaf = bpt->root->children[2];
	
	bptree_leaf_borrow_keys(bpt, left_leaf, right_leaf, left_leaf, 2, 0);
	int expected_root_keys[] = {12, 20};
	bptree_node_t *expected_root_children[] = {left_leaf, right_leaf, last_leaf};
	for(int i = 0; i < 2; i++){
		if (expected_root_keys[i] != bpt->root->keys[i]){
			printf("Error: bpt->root->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_root_keys[i], bpt->root->keys[i]);
			goto fail;
		}
	}
	for(int i = 0; i < 3; i++){
		if (expected_root_children[i] != bpt->root->children[i]){
			printf("Error: bpt->root->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_root_children[i], bpt->root->children[i]);
			goto fail;
		}
	}
	
	int expected_left_leaf_keys[] = {0, 1, 2, 3, 4, 5, 10, 11};
	for(int i = 0; i < 8; i++){
		if (expected_left_leaf_keys[i] != left_leaf->keys[i]){
			printf("Error: left_leaf->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_left_leaf_keys[i], left_leaf->keys[i]);
			goto fail;
		}
		if (expected_left_leaf_keys[i] != left_leaf->children[i]){
			printf("Error: left_leaf->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, (void *)expected_left_leaf_keys[i], left_leaf->children[i]);
			goto fail;
		}
	}
	
	int expected_right_leaf_keys[] = {12, 13, 14, 15};
	for(int i = 0; i < 4; i++){
		if (expected_right_leaf_keys[i] != right_leaf->keys[i]){
			printf("Error: right_leaf->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_right_leaf_keys[i], right_leaf->keys[i]);
			goto fail;
		}
		if (expected_right_leaf_keys[i] != right_leaf->children[i]){
			printf("Error: right_leaf->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, (void *)expected_right_leaf_keys[i], right_leaf->children[i]);
			goto fail;
		}
	}
	
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

int main(int argc, char *argv[]){
	do_single_test("check borrow_keys from left leaf", testcase_check_leaf_borrow_keys_from_left);
	do_single_test("check borrow_keys from right leaf", testcase_check_leaf_borrow_keys_from_right);
}
