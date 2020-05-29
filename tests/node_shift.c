#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_check_leaf_shift_left(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)0, (void *)0x0);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)1, (void *)0x1);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)2, (void *)0x2);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)3, (void *)0x3);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)4, (void *)0x4);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)5, (void *)0x5);
	
	bptree_node_shift(bpt, leaf, -3);
	
	if (bptree_leaf_get_key_count(bpt, leaf) != 6){
		printf("Error: Unexpected key count (expected: 6, actual: %d)\n", leaf->used);
		goto fail;
	}
	
	int expected[] = {3, 4, 5};
	for (int i = 0; i < 3; i++){
		if (bptree_leaf_get_key_by_index(bpt, leaf, i) != (bptree_key_t)expected[i]){
			printf("Error: Unexpected key value at index = %d on leaf\n", i);
			goto fail;
		}
		if (bptree_leaf_get_element_by_index(bpt, leaf, i) != (void *)expected[i]){
			printf("Error: Unexpected element value at index = %d on leaf\n", i);
			goto fail;
		}
	}
	
	if (bptree_leaf_get_element_by_index(bpt, leaf, bpt->nkeys) != (void *)NULL){
		printf("Error: Unexpected element value at index = -1 on leaf\n");
		goto fail;
	}
	
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_free(bpt);
	bptree_print(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_leaf_shift_right(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *leaf = bpt->root;
	
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)0, (void *)0x0);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)1, (void *)0x1);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)2, (void *)0x2);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)3, (void *)0x3);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)4, (void *)0x4);
	bptree_leaf_insert(bpt, leaf, (bptree_key_t)5, (void *)0x5);
	
	bptree_node_shift(bpt, leaf, 4);
	
	if (bptree_leaf_get_key_count(bpt, leaf) != 6){
		printf("Error: Unexpected key count (expected: 6, actual: %d)\n", leaf->used);
		goto fail;
	}
	
	
	int expected[] = {0, 1, 2, 3, 4, 5};
	for (int i = 0; i < 6; i++){
		if (bptree_leaf_get_key_by_index(bpt, leaf, i + 4) != (bptree_key_t)expected[i]){
			printf("Error: Unexpected key value at index = %d on leaf\n", i + 4);
			goto fail;
		}
		if (bptree_leaf_get_element_by_index(bpt, leaf, i + 4) != (void *)expected[i]){
			printf("Error: Unexpected element value at index = %d on leaf\n", i + 4);
			goto fail;
		}
	}
	
	if (bptree_leaf_get_element_by_index(bpt, leaf, bpt->nkeys) != (void *)NULL){
		printf("Error: Unexpected element value at index = -1 on leaf\n");
		goto fail;
	}
	
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_leaf_print(bpt, leaf);
	printf("\n");
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_node_shift_left(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	
	for (int i = 0; i < 6; i++){
		node->keys[i] = (bptree_key_t)i;
		node->children[i] = (void *)i;
		node->used += 1;
	}
	node->children[node->used] = (void *)6;
	
	bptree_node_shift(bpt, node, -3);
	
	if (bptree_node_get_key_count(bpt, node) != 6){
		printf("Error: Unexpected key count (expected: 6, actual: %d)\n", node->used);
		goto fail;
	}
	
	
	int expected[] = {3, 4, 5};
	for (int i = 0; i < 3; i++){
		if (bptree_node_get_key_by_index(bpt, node, i) != (bptree_key_t)expected[i]){
			printf("Error: Unexpected key value at index = %d on node\n", i);
			goto fail;
		}
		if (bptree_node_get_children_by_index(bpt, node, i) != (void *)expected[i]){
			printf("Error: Unexpected children value at index = %d on node\n", i);
			goto fail;
		}
	}
	
	if (bptree_node_get_children_by_index(bpt, node, node->used) != (void *)6){
		printf("Error: Unexpected children value at index = -1 on node\n");
		goto fail;
	}
	
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_leaf_print(bpt, node);
	printf("\n");
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_node_shift_right(){
	bptree_t *bpt = bptree_init(10);
	bptree_node_t *node = bptree_node_create(bpt);
	
	for (int i = 0; i < 6; i++){
		node->keys[i] = (bptree_key_t)i;
		node->children[i] = (void *)i;
		node->used += 1;
	}
	node->children[node->used] = (void *)6;
	
	bptree_node_shift(bpt, node, 4);
	
	if (bptree_node_get_key_count(bpt, node) != 6){
		printf("Error: Unexpected key count (expected: 6, actual: %d)\n", node->used);
		goto fail;
	}
	
	
	int expected[] = {0, 1, 2, 3, 4, 5};
	for (int i = 0; i < 6; i++){
		if (bptree_node_get_key_by_index(bpt, node, i + 4) != (bptree_key_t)expected[i]){
			printf("Error: Unexpected key value at index = %d on node\n", i + 4);
			goto fail;
		}
		if (bptree_node_get_children_by_index(bpt, node, i + 4) != (void *)expected[i]){
			printf("Error: Unexpected children value at index = %d on node\n", i + 4);
			goto fail;
		}
	}
	
	if (bptree_node_get_children_by_index(bpt, node, 10) != (void *)6){
		printf("Error: Unexpected children value at index = -1 on node\n");
		goto fail;
	}
	
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_leaf_print(bpt, node);
	printf("\n");
	bptree_node_destroy(node);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

int main(int argc, char *argv[]){
	do_single_test("check node shift left", testcase_check_leaf_shift_left);
	do_single_test("check node shift right", testcase_check_leaf_shift_right);
	do_single_test("check node shift left", testcase_check_node_shift_left);
	do_single_test("check node shift right", testcase_check_node_shift_right);
}
