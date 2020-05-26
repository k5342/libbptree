#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testutils.h"

bptree_test_context *bptree_create_test_context(int size, int data_counts){
	bptree_test_context *c = malloc(sizeof(bptree_test_context));
	c->bpt = bptree_init(size);
	c->data_counts = data_counts;
	c->values = calloc(data_counts, sizeof(int));
	return c;
}

void bptree_destroy_test_context(bptree_test_context *c){
	bptree_free(c->bpt);
	free(c->values);
	free(c);
}

void do_single_test(char *test_name, bptree_test_result (*test_func)()){
	printf("check - %s\n", test_name);
	bptree_test_result r = test_func();
	if (r == BPTREE_TEST_PASSED){
		printf("passed - %s\n", test_name);
		return;
	} else {
		printf("failed - %s\n", test_name);
		exit(1);
	}
}

int lcg(int prev, int n){
	return (prev * 3 + 41) % n;
}

bptree_test_result check_search(bptree_test_context *c){
	for (int i = 0; i < c->data_counts; i++){
		void *v;
		if (bptree_search(c->bpt, (bptree_key_t)c->values[i], &v)){
			if ((int *)v != &c->values[i]){
				printf("Error: got data is not matched (expected: %p, actual: %p)\n", &c->values[i], (int *)v);
				return BPTREE_TEST_FAILED;
			}
		} else {
			printf("Error: expected data (= key: %lld / value: %p) is not found\n", (bptree_key_t)c->values[i], (int *)v);
			return BPTREE_TEST_FAILED;
		}
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result insert_in_asc(bptree_test_context *c){
	for (int i = 0; i < c->data_counts; i++){
		c->values[i] = i;
		bptree_insert(c->bpt, (bptree_key_t)c->values[i], &c->values[i]);
		if (check_tree_structure(c) == BPTREE_TEST_FAILED){
			printf("Error: check_tree_structure() returned fail\n");
			return BPTREE_TEST_FAILED;
		}
		bptree_print(c->bpt);
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result insert_in_desc(bptree_test_context *c){
	for (int i = c->data_counts - 1; i >= 0; i--){
		c->values[i] = i;
		bptree_insert(c->bpt, (bptree_key_t)c->values[i], &c->values[i]);
		if (check_tree_structure(c) == BPTREE_TEST_FAILED){
			printf("Error: check_tree_structure() returned fail\n");
			return BPTREE_TEST_FAILED;
		}
		bptree_print(c->bpt);
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result insert_in_random(bptree_test_context *c){
	int r = lcg(51, c->data_counts);
	for (int i = 0; i < c->data_counts; i++){
		r = lcg(r, c->data_counts);
		for (int j = 0;; j++){
			void *v;
			if (!bptree_search(c->bpt, (bptree_key_t)r + j, &v)){
				c->values[i] = r + j;
				bptree_insert(c->bpt, (bptree_key_t)c->values[i], &c->values[i]);
				break;
			}
		}
		if (check_tree_structure(c) == BPTREE_TEST_FAILED){
			printf("Error: check_tree_structure() returned fail\n");
			return BPTREE_TEST_FAILED;
		}
		bptree_print(c->bpt);
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result delete_in_asc(bptree_test_context *c){
	for (int i = 0; i < c->data_counts; i++){
		bptree_delete(c->bpt, (bptree_key_t)c->values[i]);
		if (check_tree_structure(c) == BPTREE_TEST_FAILED){
			printf("Error: check_tree_structure() returned fail\n");
			return BPTREE_TEST_FAILED;
		}
		bptree_print(c->bpt);
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result _check_tree_structure(bptree_test_context *c, bptree_node_t *node, bptree_node_t *node_parent, bptree_key_t *max_key){
	if (node == NULL){
		printf("Error: Passed pointer of node is NULL\n");
		return BPTREE_TEST_FAILED;
	}
//	printf("node: %p (%p), node->parent = %p (%p), node_parent = %p (%p)\n", node, &node, node->parent, &node->parent, node_parent, &node_parent);
	printf("node: %p\n", node);
	printf("node->parent: %p\n", &node->parent);
	bptree_leaf_print(c->bpt, node);
	printf("\n");
	if (node->parent != node_parent){
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
		return BPTREE_TEST_FAILED;
	}
	// check attributes
	if (node->used > c->bpt->nkeys){
		printf("Error: node->used exceeds node->nkeys\n");
		return BPTREE_TEST_FAILED;
	}
	if (node == c->bpt->root){
		if (node->used < 0){
			printf("Error: node->used must be greater than zero for root node\n");
			return BPTREE_TEST_FAILED;
		}
	} else {
		if (node->used <= 0){
			printf("Error: node->used must be greater than zero for non-root node\n");
			return BPTREE_TEST_FAILED;
		}
	}
	for (int i = 1; i < node->used; i++){
		if (node->keys[i - 1] >= node->keys[i]){
			printf("Error: node->keys does not accomplish ascending order\n");
			printf("node: ");
			bptree_leaf_print(c->bpt, node);
			printf("\n");
			return BPTREE_TEST_FAILED;
		}
	}
	if (node->is_leaf == 1){
		if (node->used == 0){
			if (node == c->bpt->root){
				return BPTREE_TEST_PASSED;
			} else {
				return BPTREE_TEST_FAILED;
			}
		} else {
			*max_key = node->keys[node->used - 1];
			return BPTREE_TEST_PASSED;
		}
	} else if (node->is_leaf == 0){
		// check children
		for (int i = 0; i < node->used; i++){
			if (node->children[i] == NULL){
				printf("Error: node->children[%d] is NULL\n", i);
				return BPTREE_TEST_FAILED;
			}
	printf("#node: %p\n", node);
	printf("#node->parent: %p\n", &node->parent);
	bptree_leaf_print(c->bpt, node);
	printf("\n");
			int ret = _check_tree_structure(c, node->children[i], node, max_key);
			if (ret != BPTREE_TEST_PASSED){
				printf("Error: _check_tree_structure does not end successfully\n");
				return BPTREE_TEST_FAILED;
			}
			if (*max_key > node->keys[i]){
				printf("Error: node->keys[%d] (= %lld) is greater than max_key (= %lld)\n", i, node->keys[i], *max_key);
				printf("node: ");
				bptree_node_print(c->bpt, node);
				printf("\n");
				printf("tree: ");
				bptree_print(c->bpt);
				return BPTREE_TEST_FAILED;
			}
		}
		if (node->children[node->used] == NULL){
			printf("Error: node->children[-1] is NULL\n");
			printf("node: ");
			bptree_leaf_print(c->bpt, node);
			printf("\n");
			return BPTREE_TEST_FAILED;
		}
	printf("!node: %p\n", node);
	printf("!node->parent: %p\n", &node->parent);
	bptree_leaf_print(c->bpt, node);
	printf("\n");
		int ret = _check_tree_structure(c, node->children[node->used], node, max_key);
		if (ret != BPTREE_TEST_PASSED){
			printf("Error: _check_tree_structure does not end successfully\n");
			return BPTREE_TEST_FAILED;
		}
		if (node->keys[node->used - 1] > *max_key){
			printf("Error: node->keys[%d] (= %lld) is greater than max_key (= %lld)\n", node->used - 1, node->keys[node->used - 1], *max_key);
			printf("node: ");
			bptree_node_print(c->bpt, node);
			printf("\n");
			printf("tree: ");
			bptree_print(c->bpt);
			return BPTREE_TEST_FAILED;
		}
		return BPTREE_TEST_PASSED;
	} else {
		printf("Error: node->is_leaf must be set to zero or one\n");
		return BPTREE_TEST_FAILED;
	}
}

bptree_test_result check_tree_structure(bptree_test_context *c){
	if (c->bpt == NULL){
		printf("Error: bptree_init() == NULL\n");
		return BPTREE_TEST_FAILED;
	}
	if (c->bpt->root == NULL){
		printf("Error: bpt->root == NULL\n");
		return BPTREE_TEST_FAILED;
	}
	bptree_key_t max_key;
	bptree_leaf_print(c->bpt, c->bpt->root);
	printf("\n");
	return _check_tree_structure(c, c->bpt->root, c->bpt->root->parent, &max_key);
}

bptree_test_result check_min_or_max(bptree_test_context *c){
	// TODO: hang when any data is inserted
	
	// get maximum or minimum value in inserted values
	bptree_key_t min, max;
	min = max = c->values[0];
	for (int i = 1; i < c->data_counts; i++){
		if (c->values[i] < min){ min = c->values[i]; }
		if (c->values[i] > max){ max = c->values[i]; }
	}
	
	bptree_node_t *n;
	n = bptree_leaf_get_leftmost(c->bpt);
	if (n->keys[0] != min){
		printf("Error: (bptree_leaf_get_leftmost())->keys[0] is not most smallest value (expected: %lld, actual: %lld)\n", min, n->keys[0]);
		return BPTREE_TEST_FAILED;
	}
	
	n = bptree_leaf_get_rightmost(c->bpt);
	if (n->keys[n->used - 1] != max){
		printf("Error: (bptree_leaf_get_rightmost())->keys[-1] is not most largest value (expected: %lld, actual: %lld)\n", max, n->keys[n->used - 1]);
		return BPTREE_TEST_FAILED;
	}
	return BPTREE_TEST_PASSED;
}

int _cmp_asc(const void *a, const void *b){
	return *(int *)a - *(int *)b;
}

bptree_test_result check_leaf(bptree_test_context *c){
	int *buf = (int *)calloc(c->data_counts, sizeof(int));
	memcpy(buf, c->values, sizeof(int) * c->data_counts);
	qsort(buf, c->data_counts, sizeof(int), _cmp_asc);
	
	bptree_node_t *leaf = bptree_leaf_get_leftmost(c->bpt);
	int global_index = 0;
	while(1) {
		// check keys
		for(int i = 0; i < bptree_leaf_get_key_count(c->bpt, leaf); i++){
			bptree_key_t k = bptree_leaf_get_key_by_index(c->bpt, leaf, i);
			if (k != (bptree_key_t)buf[global_index]){
				printf("Error: key[%d] at leaf:%p is wrong (expected: %lld, actual: %lld)\n", i, leaf, (bptree_key_t)buf[global_index], k);
				free(buf);
				return BPTREE_TEST_FAILED;
			}
			global_index += 1;
		}
		
		// check adjacent leaf
		bptree_node_t *next;
		next = bptree_leaf_get_rightadjacent(c->bpt, leaf);
		if (next == NULL){
			if (bptree_leaf_get_rightmost(c->bpt) == leaf){
				break;
			} else {
				printf("Error: leaf:%p indicates rightmost but actual rightmost is leaf:%p\n", leaf, bptree_leaf_get_rightmost(c->bpt));
				printf("leaf:%p: ", leaf);
				bptree_leaf_print(c->bpt, leaf);
				printf("\n");
				printf("leaf:%p: ", bptree_leaf_get_rightmost(c->bpt));
				bptree_leaf_print(c->bpt, bptree_leaf_get_rightmost(c->bpt));
				printf("\n");
				free(buf);
				return BPTREE_TEST_FAILED;
			}
		} else {
			leaf = next;
		}
	}
	free(buf);
	return BPTREE_TEST_PASSED;
}

bptree_test_result verify_tree(bptree_test_context *c){
	if (check_search(c) == BPTREE_TEST_FAILED){
		printf("check_search returned fail\n");
		return BPTREE_TEST_FAILED;
	}
	if (check_min_or_max(c) == BPTREE_TEST_FAILED){
		printf("check_min_or_max returned fail\n");
		return BPTREE_TEST_FAILED;
	}
	if (check_leaf(c) == BPTREE_TEST_FAILED){
		printf("check_leaf returned fail\n");
		return BPTREE_TEST_FAILED;
	}
	return BPTREE_TEST_PASSED;
}
