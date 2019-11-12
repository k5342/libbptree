#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>

const int data_counts = 20;

typedef struct _context {
	bptree_t *bpt;
	int *values;
	int npassed;
	int nfailed;
} context;

context *create_context(int size){
	context *c = malloc(sizeof(context));
	c->bpt = bptree_init(size);
	c->values = calloc(data_counts, sizeof(int));
	c->npassed = 0;
	c->nfailed = 0;
	return c;
}

void *destroy_context(context *c){
	bptree_free(c->bpt);
	free(c);
}

int do_test(char *print_string, void (*prepare)(context *c), void (*verify)(context *c)){
	context *c = create_context(10);
	
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

int check_search(context *c){
	for (int i = 0; i < data_counts; i++){
		int status;
		int *v = bptree_search(c->bpt, (bptree_key_t)i, &status);
		if (status){
			if (v == &c->values[i]){
				// ok
				c->npassed += 1;
			} else {
				printf("(%d/%d) ERROR / found = %p, correct = %p\n", i, data_counts, v, &c->values[i]);
				c->nfailed += 1;
			}
		} else {
			printf("(%d/%d) ERROR / not found\n", i, data_counts);
			c->nfailed += 1;
		}
	}
}

void insert_in_asc(context *c){
	for (int i = 0; i < data_counts; i++){
		bptree_insert(c->bpt, (bptree_key_t)i, &c->values[i]);
	}
}

void insert_in_desc(context *c){
	for (int i = data_counts - 1; i >= 0; i--){
		bptree_insert(c->bpt, (bptree_key_t)i, &c->values[i]);
	}
}

void insert_in_random(context *c){
	int r = lcg(51, data_counts);
	for (int i = 0; i < data_counts; i++){
		r = lcg(r, data_counts);
		for (int j = 0;; j++){
			int status;
			bptree_search(c->bpt, (bptree_key_t)r + j, &status);
			if (status == 0){
				bptree_insert(c->bpt, (bptree_key_t)r + j, &c->values[i]);
				break;
			}
		}
	}
}

void verify(context *c){
	check_search(c);
}

int main(int argc, char *argv[]){
	printf("libptree test: insert\n");
	
	int is_failed = 0;
	is_failed |= do_test("insert in ascending order", insert_in_asc, verify);
	is_failed |= do_test("insert in descending order", insert_in_desc, verify);
	is_failed |= do_test("insert in random order", insert_in_random, verify);
	
	printf("Overall Result: ");
	if (is_failed){
		printf("FAILED\n");
	} else {
		printf("PASSED\n");
	}
	return is_failed;
}
