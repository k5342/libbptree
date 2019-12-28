#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

const int data_counts = 200;

bptree_test_context *bptree_create_test_context(int size){
	bptree_test_context *c = malloc(sizeof(bptree_test_context));
	c->bpt = bptree_init(size);
	c->values = calloc(data_counts, sizeof(int));
	c->npassed = 0;
	c->nfailed = 0;
	return c;
}

void *bptree_destroy_context(bptree_test_context *c){
	bptree_free(c->bpt);
	free(c);
}

void do_single_test(char *test_name, bptree_test_result (*test_func)()){
	bptree_test_result r = test_func();
	if (r == BPTREE_TEST_PASSED){
		printf("passed - %s\n", test_name);
		return;
	} else {
		printf("failed - %s\n", test_name);
		exit(1);
	}
}

bptree_test_result do_test(char *print_string, 
		bptree_test_result (*prepare)(bptree_test_context *c), 
		bptree_test_result (*verify)(bptree_test_context *c)){
	bptree_test_context *c = bptree_create_test_context(10);
	
	printf("[TEST] %s\n", print_string);
	prepare(c);
	verify(c);
	
	if (c->nfailed == 0){
		printf("Result: PASSED - ");
	} else {
		printf("Result: FAILED - ");
	}
	printf("passed: %d, failed: %d\n", c->npassed, c->nfailed);
	if (c->nfailed == 0){
		return 0;
	} else {
		bptree_print(c->bpt);
		return 1;
	}
}

int lcg(int prev, int n){
	return (prev * 3 + 41) % n;
}

bptree_test_result check_search(bptree_test_context *c){
	for (int i = 0; i < data_counts; i++){
		void *v;
		if (bptree_search(c->bpt, (bptree_key_t)c->values[i], &v)){
			if ((int *)v == &c->values[i]){
				// ok
				c->npassed += 1;
			} else {
				printf("(%d/%d) ERROR / key = %d, found = %p, correct = %p\n", i, data_counts, c->values[i], v, &c->values[i]);
				c->nfailed += 1;
			}
		} else {
			printf("(%d/%d) ERROR / key = %d, not found\n", i, data_counts, c->values[i]);
			c->nfailed += 1;
		}
	}
	if (c->nfailed > 0){
		return BPTREE_TEST_FAILED;
	} else {
		return BPTREE_TEST_PASSED;
	}
}

bptree_test_result insert_in_asc(bptree_test_context *c){
	for (int i = 0; i < data_counts; i++){
		c->values[i] = i;
		bptree_insert(c->bpt, (bptree_key_t)c->values[i], &c->values[i]);
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result insert_in_desc(bptree_test_context *c){
	for (int i = data_counts - 1; i >= 0; i--){
		c->values[i] = i;
		bptree_insert(c->bpt, (bptree_key_t)c->values[i], &c->values[i]);
		if (verify_tree(c) == BPTREE_TEST_FAILED){
			return BPTREE_TEST_FAILED;
		}
		bptree_print(c->bpt);
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result insert_in_random(bptree_test_context *c){
	int r = lcg(51, data_counts);
	for (int i = 0; i < data_counts; i++){
		r = lcg(r, data_counts);
		for (int j = 0;; j++){
			void *v;
			if (!bptree_search(c->bpt, (bptree_key_t)r + j, &v)){
				c->values[i] = r + j;
				bptree_insert(c->bpt, (bptree_key_t)c->values[i], &c->values[i]);
				break;
			}
		}
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result _verify_node(bptree_test_context *c, bptree_node_t *node, bptree_node_t *node_parent, bptree_key_t *max_key){
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
	if (node->parent == node_parent){
		c->npassed += 1;
	} else {
		printf("Error: node->parent is not correct (expected %p, but %p)\n", node_parent, node->parent);
		printf("node: \n");
		bptree_leaf_print(c->bpt, node);
		printf("\n");
		printf("expected: \n");
		bptree_leaf_print(c->bpt, node_parent);
		printf("\n");
		printf("actual: \n");
		bptree_leaf_print(c->bpt, node->parent);
		printf("\n");
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
			int ret = _verify_node(c, node->children[i], node, max_key);
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
		int ret = _verify_node(c, node->children[node->used], node, max_key);
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
		return BPTREE_TEST_PASSED;
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
	return _verify_node(c, c->bpt->root, c->bpt->root->parent, &max_key);
}

