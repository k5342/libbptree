#include <bptree.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	bptree_t *bpt = bptree_init(10);
	if (bpt == NULL){
		bptree_perror("bptree_init returned NULL");
		return -1;
	}
	char buffer[128]; 
	printf("> ");
	while (fgets(buffer, 256, stdin) != NULL){
		char *t = strtok(buffer, " ");
		if (strncmp(t, "p", 1) == 0
			|| strncmp(t, "put", 3) == 0){
			t = strtok(NULL, " ");
			bptree_key_t key = (bptree_key_t)strtol(t, NULL, 10);
			bptree_insert(bpt, key, NULL);
			bptree_print(bpt);
			goto next;
		}
		if (strncmp(t, "d", 1) == 0
			|| strncmp(t, "del", 3) == 0
			|| strncmp(t, "delete", 6) == 0){
			t = strtok(NULL, " ");
			bptree_key_t key = (bptree_key_t)strtol(t, NULL, 10);
			bptree_delete(bpt, key);
			bptree_print(bpt);
			goto next;
		}
		if (strncmp(t, "show", 4) == 0){
			bptree_print(bpt);
			goto next;
		}
		if (strncmp(t, "help", 4) == 0){
			goto put_usage;
		}
		if (strncmp(t, "exit", 4) == 0){
			goto exit;
		}
put_usage:
		printf(
			"Available Commands:\n"
			"  put <number>\n"
			"  delete <number>\n"
			"  show\n"
			"  exit\n"
		);
next:
		printf("> ");
	}
exit:
	bptree_free(bpt);
	return 0;
}
