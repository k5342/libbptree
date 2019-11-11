#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>

const int data_counts = 11;

int hash(int n){
	return 0;
}

int check(bptree_t *bpt, int *vals){
	for (int i = 0; i < data_counts; i++){
		int status;
		int *v = bptree_search(bpt, (bptree_key_t)i, &status);
		if (status){
			if (v == &vals[i]){
				// ok
			} else {
				printf("(%d/%d) ERROR / found = %p (= %d), correct = %d\n", i, data_counts, v, *v, vals[i]);
			}
		} else {
			printf("(%d/%d) ERROR / not found = %d\n", i, data_counts, hash(i));
		}
	}
	printf("test success.\n");
}

int main(int argc, char *argv[]){
	bptree_t *bpt = bptree_init(10);
	if (bpt == NULL){
		bptree_perror("bptree_init returned NULL");
		return -1;
	}
	
	// initialize values
	int *vals = calloc(data_counts, sizeof(int));
	
	printf("insert in ascending order\n");
	printf("insert ...");
	for (int i = 0; i < data_counts; i++){
		vals[i] = i;
		bptree_insert(bpt, (bptree_key_t)i, &vals[i]);
	}
	printf("done\n");
	bptree_print(bpt);
	printf("check ...\n");
	check(bpt, vals);
	
	printf("insert in random order\n");
	printf("insert ...");
	for (int i = 0; i < data_counts; i++){
		for (int j = 0;; j++){
			int status;
			bptree_search(bpt, (bptree_key_t)hash(i) + j, &status);
			if (status == 0){
				vals[i] = hash(i) + j;
				bptree_insert(bpt, (bptree_key_t)hash(i) + j, &vals[i]);
				break;
			}
		}
	}
	printf("done\n");
	bptree_print(bpt);
	printf("check ...\n");
	check(bpt, vals);
	
	bptree_free(bpt);
	
	return 0;
}
