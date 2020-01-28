#include <bptree.h>
#include <string.h>
#include <stdlib.h>

void put_status(bptree_t *bpt){
	if (bpt == NULL){
		printf("bpt = (nil)\n");
	} else {
		printf("bpt = %p\n", bpt);
		bptree_print(bpt);
	}
	return;
}

int main(int argc, char *argv[]){
	bptree_t *bpt;
	
	char buffer[128]; 
	printf("> ");
	while (fgets(buffer, 256, stdin) != NULL){
		char *t = strtok(buffer, " ");
		if (strncmp(t, "init", 4) == 0){
			t = strtok(NULL, " ");
			bptree_key_t nkeys = (bptree_key_t)strtol(t, NULL, 10);
			bpt = bptree_init(nkeys);
			if (bpt == NULL){
				bptree_perror("bptree_init returned NULL");
			} else {
				put_status(bpt);
			}
			goto next;
		}
		if (strncmp(t, "destroy", 7) == 0){
			if (bpt == NULL){
				printf("not initialized\n");
			} else {
				bptree_free(bpt);
				bpt = NULL;
			}
			put_status(bpt);
			goto next;
		}
		if (strncmp(t, "p", 1) == 0
			|| strncmp(t, "put", 3) == 0){
			t = strtok(NULL, " ");
			if (bpt == NULL){
				printf("You should call init command first\n");
			} else {
				bptree_key_t key = (bptree_key_t)strtol(t, NULL, 10);
				bptree_insert(bpt, key, NULL);
				put_status(bpt);
			}
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
			put_status(bpt);
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
			"  init <number>\n"
			"  put <number>\n"
			"  delete <number>\n"
			"  show\n"
			"  destroy\n"
			"  exit\n"
		);
next:
		printf("> ");
	}
exit:
	if (bpt != NULL){
		bptree_free(bpt);
	}
	return 0;
}
