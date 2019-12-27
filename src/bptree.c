#include <bptree.h>

void bptree_perror(char *s){
	if (s == NULL){
		perror("bptree: perror: s is NULL\n");
		return;
	}
	fprintf(stderr, "bptree: %s\n", s);
}

void bptree_leaf_print(bptree_t *bpt, bptree_node_t *leaf){
	printf("{");
	if (leaf->used > 0){
		for(int i = 0; i < leaf->used; i++){
			printf("%p %lld ", leaf->children[i], leaf->keys[i]);
		}
		printf("%p", leaf->children[leaf->used]);
	}
	printf("}");
}

void bptree_node_print(bptree_t *bpt, bptree_node_t *node){
	if (node->is_leaf == true){
		bptree_leaf_print(bpt, node);
	} else {
		bptree_node_t *current = node;
		printf("[");
		if (current->used > 0){
			for(int i = 0; i < current->used; i++){
				if (current->children[i] == NULL){
					printf("(NULL) ");
				} else {
					bptree_node_print(bpt, current->children[i]);
					printf(" ");
				}
				printf("%lld ", current->keys[i]);
			}
			if (current->children[current->used] == NULL){
				printf("(NULL)");
			} else {
				bptree_node_print(bpt, current->children[current->used]);
			}
		}
		printf("]");
	}
}

void bptree_print(bptree_t *bpt){
	bptree_node_print(bpt, bpt->root);
	printf("\n");
}

bptree_node_t *bptree_node_create(bptree_t *bpt){
	if (bpt == NULL){
		bptree_perror("bpt is NULL");
		return NULL;
	}
	int nkeys = bpt->nkeys;
	bptree_node_t *node = (bptree_node_t *)malloc(sizeof(bptree_node_t));
	memset(node, 0, sizeof(bptree_node_t));
	node->context = bpt;
	node->parent = NULL;
	node->children = (bptree_node_t **)malloc(sizeof(bptree_node_t *) * (nkeys + 1));
	node->keys = (bptree_key_t *)malloc(sizeof(bptree_key_t) * nkeys);
	node->used = 0;
	node->is_leaf = false;
	return node;
}

bptree_node_t *bptree_leaf_create(bptree_t *bpt){
	bptree_node_t *leaf = bptree_node_create(bpt);
	if (leaf == NULL){
		return NULL;
	}
	leaf->is_leaf = true;
	leaf->children[bpt->nkeys] = NULL;
	return leaf;
}

void bptree_node_destroy(bptree_node_t *node){
	if (node == NULL){
		bptree_perror("node is NULL");
		return;
	}
	free(node->children);
	free(node->keys);
	free(node);
}

int bptree_node_insert_index(bptree_t *bpt, bptree_node_t *node, bptree_key_t key){
	if (node->used <= 0){
		return 0;
	}
	for (int i = 0; i < node->used && i < bpt->nkeys; i++){
#ifdef DEBUG
		printf("keys[%d] = %lld\n", i, node->keys[i]);
#endif
		if (node->keys[i] > key){
			return i;
		}
	}
	return node->used;
}

void bptree_node_insert(bptree_t *bpt, bptree_node_t *l_child, bptree_key_t key, bptree_node_t *r_child){
#ifdef DEBUG
	printf("insert into node: key = %lld\n", key);
	bptree_node_print(bpt, r_child);
#endif
	bptree_node_t *dst_node = l_child->parent;
	if (dst_node == NULL){
		// divide root
#ifdef DEBUG
		printf("divide root: used = %d\n", l_child->used);
#endif
		bptree_node_t *new_root = bptree_node_create(bpt);
		new_root->keys[0] = key;
		new_root->children[0] = l_child;
		new_root->children[1] = r_child;
		new_root->used = 1;
		l_child->parent = new_root;
		r_child->parent = new_root;
		bpt->root = new_root;
		return;
	}
	
	int insert_index = bptree_node_insert_index(bpt, dst_node, key);
#ifdef DEBUG
	printf("node_insert: index = %d, r_child = %p\n", insert_index, r_child);
#endif
	if (dst_node->used + 1 <= bpt->nkeys){
		// shift and insert
		for (int i = dst_node->used + 1; i > insert_index; i--){
			dst_node->keys[i] = dst_node->keys[i - 1];
			dst_node->children[i + 1] = dst_node->children[i];
		}
		dst_node->keys[insert_index] = key;
		dst_node->children[insert_index + 1] = r_child;
		dst_node->used += 1;
		r_child->parent = dst_node;
		return;
	} else {
		// copy
#ifdef DEBUG
		printf("divide node\n");
#endif
		bptree_node_t *tmp_children[bpt->nkeys + 1 + 1]; // TODO: optimize this
		bptree_key_t tmp_keys[bpt->nkeys + 1];
		
		// insert key into temp node
		for (int i = 0; i < insert_index; i++){
			tmp_keys[i] = dst_node->keys[i];
		}
		tmp_keys[insert_index] = key;
		for (int i = insert_index; i < dst_node->used; i++){
			// shift +1 after the key insertion
			tmp_keys[i + 1] = dst_node->keys[i];
		}
		
		// insert r_child into temp node
		// it should be careful the r_child insertion on internal node is occurred at insert_index + 1
		// because key ensures the maximum key in children[i] is less than key[i]
		for (int i = 0; i < insert_index + 1; i++){
			tmp_children[i] = dst_node->children[i];
		}
		tmp_children[insert_index + 1] = r_child;
		for (int i = insert_index + 1; i < dst_node->used + 1; i++){
			tmp_children[i + 1] = dst_node->children[i];
		}
		
		// divide into left and right node
		// first, create right node
		bptree_node_t *new = bptree_node_create(bpt);
		
		int median_index = (bpt->nkeys + 1) / 2;
		int divide_left_nkeys = median_index;
		int divide_right_nkeys = (bpt->nkeys + 1) - median_index - 1;
		
		// copy from temp node to left node
		dst_node->used = 0;
		for(int i = 0; i < divide_left_nkeys; i++){
			dst_node->keys[i] = tmp_keys[i];
			dst_node->children[i] = tmp_children[i];
			dst_node->used += 1;
		}
		dst_node->children[divide_left_nkeys] = tmp_children[divide_left_nkeys];
		
		// copy from temp node to right (new) node
		new->used = 0;
		for(int i = 0; i < divide_right_nkeys; i++){
			new->keys[i] = tmp_keys[i + median_index + 1];
			new->children[i] = tmp_children[i + median_index + 1];
			new->children[i]->parent = new;
			new->used += 1;
		}
		new->children[divide_right_nkeys] = tmp_children[divide_right_nkeys + median_index + 1];
		new->children[divide_right_nkeys]->parent = new;
		
		// set r_child->parent which determined by the value of median_index
		if (insert_index < median_index){
			r_child->parent = dst_node;
		} else {
			r_child->parent = new;
		}
		
		bptree_node_insert(bpt, dst_node, tmp_keys[median_index], new);
		return;
	}
}

bptree_node_t *bptree_leaf_search(bptree_t *bpt, bptree_key_t key){
	bptree_node_t *current = bpt->root;
	while (current->is_leaf == false){
		if (current->used <= 0){
			return current;
		}
		int idx;
		for (idx = 0; idx < current->used; idx++){
#ifdef DEBUG
			printf("comp: %lld > %lld\n", current->keys[idx], key);
#endif
			if (current->keys[idx] > key){
				break;
			}
		}
		current = current->children[idx];
	}
	return current;
}

void bptree_leaf_insert(bptree_t *bpt, bptree_node_t *leaf, bptree_key_t key, void *value){
	int insert_index = bptree_node_insert_index(bpt, leaf, key);
	if (leaf->used + 1 <= bpt->nkeys){
		// shift and insert
		for (int i = leaf->used; i > insert_index; i--){
			leaf->keys[i] = leaf->keys[i - 1];
			leaf->children[i] = leaf->children[i - 1];
		}
		leaf->keys[insert_index] = key;
		leaf->children[insert_index] = (bptree_node_t *)value;
		leaf->used += 1;
	} else {
#ifdef DEBUG
		printf("divide leaf: key = %lld\n", key);
#endif
		// create temp node for leaf separation
		bptree_node_t *tmp_children[bpt->nkeys + 1]; // TODO: optimize this
		bptree_key_t tmp_keys[bpt->nkeys + 1];
		
		// insert into temp node
		for (int i = 0; i < insert_index; i++){
			tmp_keys[i] = leaf->keys[i];
			tmp_children[i] = leaf->children[i];
		}
		tmp_keys[insert_index] = key;
		tmp_children[insert_index] = (bptree_node_t *)value;
		for (int i = insert_index; i < leaf->used; i++){
			tmp_keys[i + 1] = leaf->keys[i];
			tmp_children[i + 1] = leaf->children[i];
		}
		
		// divide into left and right node
		bptree_node_t *new = bptree_leaf_create(bpt);
		leaf->children[bpt->nkeys] = new;
		
		int divide_right_size = (bpt->nkeys + 1) / 2;
		int divide_left_size = (bpt->nkeys + 1) - divide_right_size;
		
		// copy from temp node
		for(int i = 0; i < divide_left_size; i++){
			leaf->keys[i] = tmp_keys[i];
			leaf->children[i] = tmp_children[i];
		}
		leaf->used = divide_left_size;
		
		for(int i = 0; i < divide_right_size; i++){
			new->keys[i] = tmp_keys[i + divide_left_size];
			new->children[i] = tmp_children[i + divide_left_size];
		}
		new->used = divide_right_size;
		
		// insert new node into B+-tree
		bptree_node_insert(bpt, leaf, new->keys[0], new);
	}
}

// search B+-tree passed by bpt
// returns 1 (if found)
// returns 0 (if not found)
int bptree_search(bptree_t *bpt, bptree_key_t key, void **ptr){
	bptree_node_t *leaf = bptree_leaf_search(bpt, key);
	for (int i = 0; i < leaf->used; i++){
		if (key == leaf->keys[i]){
			*ptr = leaf->children[i];
			return 1;
		}
	}
	*ptr = NULL;
	return 0;
}

void bptree_insert(bptree_t *bpt, bptree_key_t key, void *value){
	bptree_node_t *leaf = bptree_leaf_search(bpt, key);
	bptree_leaf_insert(bpt, leaf, key, value);
}

bptree_t *bptree_init(int nkeys){
	if (nkeys <= 0){
		bptree_perror("nkeys must be bigger than zero");
		return NULL;
	}
	// create bptree object
	bptree_t *bpt = (bptree_t *)malloc(sizeof(bptree_t));
	if (bpt == NULL){
		bptree_perror("bptree_init: malloc returned NULL");
		return NULL;
	}
	memset(bpt, 0, sizeof(bptree_t));
	bpt->nkeys = nkeys;
	
	// create root node
	bpt->root = bptree_leaf_create(bpt);
	return bpt;
}

void bptree_free(bptree_t *bpt){
	if (bpt == NULL){
		bptree_perror("bptree_free: bpr is NULL");
	}
	free(bpt->root);
	free(bpt);
}

