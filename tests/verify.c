#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result _verify_node(bptree_test_context *c, bptree_node_t *node, bptree_key_t *max_key){
	if (node == NULL){
		printf("Error: Passed pointer of node is NULL\n");
		c->nfailed += 1;
		return BPTREE_TEST_FAILED;
	}
	// check attributes
	if (node->used > 0){
		c->npassed += 1;
	} else {
		printf("Error: node->used must be greater than zero\n");
		c->nfailed += 1;
		return BPTREE_TEST_FAILED;
	}
	if (node->used <= c->bpt->nkeys){
		c->npassed += 1;
	} else {
		printf("Error: node->used exceeds node->nkeys\n");
		c->nfailed += 1;
		return BPTREE_TEST_FAILED;
	}
	for (int i = 1; i < node->used; i++){
		if (node->keys[i - 1] < node->keys[i]){
			c->npassed += 1;
		} else {
			printf("Error: node->keys does not accomplish ascending order\n");
			c->nfailed += 1;
			return BPTREE_TEST_FAILED;
		}
	}
	if (node->is_leaf == 0 || node->is_leaf == 1){
		c->npassed += 1;
	} else {
		printf("Error: node->is_leaf must be set to zero or one\n");
		c->nfailed += 1;
		return BPTREE_TEST_FAILED;
	}
	if (node->is_leaf == 1){
		*max_key = node->keys[node->used - 1];
		return BPTREE_TEST_PASSED;
	}
	if (node->is_leaf == 0){
		// check children
		for (int i = 0; i < node->used; i++){
			if (node->children[i] == NULL){
				printf("Error: node->children[%d] is NULL\n", i);
				c->nfailed += 1;
				return BPTREE_TEST_FAILED;
			} else {
				c->npassed += 1;
			}
			int ret = _verify_node(c, node->children[i], max_key);
			if (ret != 0){
				printf("Error: _verify_node does not end successfully\n");
				return BPTREE_TEST_FAILED;
			}
			if (*max_key < node->keys[i]){
				c->npassed += 1;
			} else {
				printf("Error: node->keys[%d] (= %lld) is greater than max_key (= %lld)\n", i, node->keys[i], *max_key);
				c->nfailed += 1;
				return BPTREE_TEST_FAILED;
			}
		}
		if (node->children[node->used] == NULL){
			c->nfailed += 1;
			printf("Error: node->children[-1] is NULL\n");
			return BPTREE_TEST_FAILED;
		} else {
			c->npassed += 1;
		}
		int ret = _verify_node(c, node->children[node->used], max_key);
		if (ret != 0){
			printf("Error: _verify_node does not end successfully\n");
			return BPTREE_TEST_FAILED;
		}
		if (node->keys[node->used - 1] < *max_key){
			c->npassed += 1;
		} else {
			printf("Error: node->keys[%d] (= %lld) is greater than max_key (= %lld)\n", node->used - 1, node->keys[node->used - 1], *max_key);
			c->nfailed += 1;
			return BPTREE_TEST_FAILED;
		}
		return 0;
	}
}

bptree_test_result verify_tree(bptree_test_context *c){
	if (c->bpt == NULL){
		printf("Error: bptree_init() == NULL\n");
		c->nfailed += 1;
		return BPTREE_TEST_FAILED;
	}
	if (c->bpt->root == NULL){
		printf("Error: bpt->root == NULL\n");
		c->nfailed += 1;
		return BPTREE_TEST_FAILED;
	}
	bptree_key_t max_key;
	return _verify_node(c, c->bpt->root, &max_key);
}

int main(int argc, char *argv[]){
	printf("libptree test: insert\n");
	
	int is_failed = 0;
	if (do_test("insert in ascending order",
				insert_in_asc, verify_tree) == BPTREE_TEST_FAILED){
		is_failed |= 1;
	}
	if (do_test("insert in descending order",
				insert_in_desc, verify_tree) == BPTREE_TEST_FAILED){
		is_failed |= 1;
	}
	if (do_test("insert in random order",
				insert_in_random, verify_tree) == BPTREE_TEST_FAILED){
		is_failed |= 1;
	}
	
	printf("Overall Result: ");
	if (is_failed){
		printf("FAILED\n");
		return 1;
	} else {
		printf("PASSED\n");
		return 0;
	}
}
