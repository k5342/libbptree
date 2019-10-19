#include <bptree.h>

int main(int argc, char *argv[]){
	setvbuf(stdout, (char *)NULL, _IONBF, 0);
	bptree_t *bpt = bptree_init(10);
	if (bpt == NULL){
		bptree_perror("bptree_init returned NULL");
		return -1;
	}
	for (int i = 100; i > 0; i--){
#ifdef DEBUG
		printf("insert %d\n", i);
#endif
		bptree_insert(bpt, (bptree_key_t)i, NULL);
		bptree_print(bpt);
	}
	bptree_free(bpt);
	return 0;
}
