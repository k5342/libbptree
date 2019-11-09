#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>

int hash(int n){
	return (n * 23 + 41) % 10000;
}

int main(int argc, char *argv[]){
	bptree_t *bpt = bptree_init(10);
	if (bpt == NULL){
		bptree_perror("bptree_init returned NULL");
		return -1;
	}
	int data_counts = 10000;
	
	// initialize values
	int *vals = calloc(data_counts, sizeof(int));
	for (int i = 0; i < data_counts; i++){
		vals[i] = hash(i);
	}
	
	// insert in random order
	for (int i = 0; i < data_counts; i++){
		printf("(%d/%d) insert (%d, %p) = %d\n", i, data_counts, hash(i), &vals[i], vals[i]);
		bptree_insert(bpt, (bptree_key_t)hash(i), &vals[i]);
	}
	for (int i = 0; i < data_counts; i++){
		int status;
		int *v = bptree_search(bpt, (bptree_key_t)hash(i), &status);
		if (status && hash(i) == vals[i]){
			printf("(%d/%d) pass.\n", i, data_counts);
		} else {
			printf("(%d/%d) ERROR. / status = %d, found = %p (= %d), correct = %d\n", i, data_counts, status, v, *v, vals[i]);
			bptree_free(bpt);
			return -1;
		}
	}
	printf("test success.");
	bptree_free(bpt);
	return 0;
}
