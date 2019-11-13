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
		int status;
		int *v = bptree_search(c->bpt, (bptree_key_t)c->values[i], &status);
		if (status){
			if (v == &c->values[i]){
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
	}
	return BPTREE_TEST_PASSED;
}

bptree_test_result insert_in_random(bptree_test_context *c){
	int r = lcg(51, data_counts);
	for (int i = 0; i < data_counts; i++){
		r = lcg(r, data_counts);
		for (int j = 0;; j++){
			int status;
			bptree_search(c->bpt, (bptree_key_t)r + j, &status);
			if (status == 0){
				c->values[i] = r + j;
				bptree_insert(c->bpt, (bptree_key_t)c->values[i], &c->values[i]);
				break;
			}
		}
	}
	return BPTREE_TEST_PASSED;
}

