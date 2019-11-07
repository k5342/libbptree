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
		for(int i = 0; i < leaf->used - 1; i++){
			printf("%lld ", leaf->keys[i]);
		}
		printf("%lld", leaf->keys[leaf->used - 1]);
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

int bptree_node_key_index(bptree_t *bpt, bptree_node_t *node, bptree_key_t key){
	for(int i = 0; i <= node->used; i++){
		if (node->keys[i] == key){
			return i;
		}
	}
	return -1;
}

// search node in ptr
int bptree_node_ptr_index(bptree_t *bpt, bptree_node_t *node, bptree_node_t *ptr){
	for(int i = 0; i < node->used; i++){
		if (node->children[i] == ptr){
			return i;
		}
	}
	return -1;
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
		new_root->keys[0] = r_child->keys[0];
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
//		  bptree_node_print(bpt, dst_node);
		for (int i = dst_node->used; i > insert_index; i--){
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
		bptree_node_t *tmp_children[bpt->nkeys + 1]; // TODO: optimize this
		bptree_key_t tmp_keys[bpt->nkeys + 1];
		
		for (int i = bpt->nkeys + 1; i > insert_index; i--){
			tmp_keys[i] = dst_node->keys[i - 1];
			tmp_children[i + 1] = dst_node->children[i];
		}
		for (int i = 0; i < insert_index; i++){
			tmp_keys[i] = dst_node->keys[i];
			tmp_children[i] = dst_node->children[i];
		}
		tmp_children[insert_index] = dst_node->children[insert_index];
		
		// insert
		tmp_keys[insert_index] = key;
		tmp_children[insert_index + 1] = r_child;
		
//		  // debug
//		  for(int i = 0; i < dst_node->used + 1; i++){
//			  printf("children[%d]: ", i);
//			  bptree_node_print(bpt, tmp_children[i]);
//			  printf("\n");
//			  printf("keys[%d]: %d", i, tmp_keys[i]);
//			  printf("\n");
//		  }
//		  bptree_node_print(bpt, tmp_children[dst_node->used + 1]);
//		  printf("\n");
		
		// divide into left and right node
		bptree_node_t *new = bptree_node_create(bpt);
		
		int _mdl = ceil((float)(bpt->nkeys + 1) / 2);
		
		// copy from temp node
		dst_node->used = 0;
		for(int i = 0; i < _mdl; i++){
			dst_node->keys[i] = tmp_keys[i];
			dst_node->children[i] = tmp_children[i];
			dst_node->used += 1;
		}
		dst_node->children[_mdl] = tmp_children[_mdl];
//		  bptree_node_print(bpt, dst_node);
		
		new->used = 0;
		for(int i = _mdl + 1; i < bpt->nkeys + 1; i++){
			new->keys[i - _mdl - 1] = tmp_keys[i];
			new->children[i - _mdl - 1] = tmp_children[i];
			new->used += 1;
		}
		new->children[bpt->nkeys + 1 - _mdl - 1] = tmp_children[bpt->nkeys + 1];
		//bptree_node_print(bpt, new);
		
		if (insert_index < _mdl){
			r_child->parent = dst_node;
		} else {
			r_child->parent = new;
		}
		
		bptree_node_insert(bpt, dst_node, tmp_keys[_mdl], new);
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
			printf("comp: %d > %d\n", current->keys[idx], key);
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
			leaf->children[i + 1] = leaf->children[i];
		}
		leaf->keys[insert_index] = key;
		leaf->children[insert_index] = (bptree_node_t *)value;
		leaf->used += 1;
	} else {
#ifdef DEBUG
		printf("divide leaf: key = %lld\n", key);
#endif
		// copy temp node
		bptree_node_t *tmp_children[bpt->nkeys + 1]; // TODO: optimize this
		bptree_key_t tmp_keys[bpt->nkeys + 1];
		
		for (int i = bpt->nkeys + 1; i > insert_index; i--){
			tmp_keys[i] = leaf->keys[i - 1];
			tmp_children[i + 1] = leaf->children[i];
		}
		for (int i = 0; i < insert_index; i++){
			tmp_keys[i] = leaf->keys[i];
			tmp_children[i] = leaf->children[i];
		}
		tmp_keys[insert_index] = key;
		tmp_children[insert_index + 1] = (bptree_node_t *)value;
		
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

// remove key and ptr from node
void bptree_node_delete(bptree_t *bpt, bptree_node_t *node, bptree_key_t key, bptree_node_t *ptr){
	int idx = bptree_node_key_index(bpt, node, key);
	if (idx < 0){
		// not found
		return;
	}
	
	// set offset for deleting ptr by whether ptr is located on left or right of key
	int offset;
	if (node->children[idx] == ptr){
		offset = 0;
	} else if (node->children[idx + 1] == ptr){
		offset = 1;
	} else {
		// not found
		return;
	}
	
	// shift elements on node to remove key and ptr
	for(int i = idx; i < node->used - 1; i++){
		node->keys[i] = node->keys[i + 1];
		node->children[i + offset] = node->children[i + offset + 1];
	}
	if (offset == 0){
		node->children[node->used - 1] = node->children[node->used];
	}
	node->used -= 1;
	
	if (node == bpt->root){
		return;
	} else {
		// if node does not satisfy B+-tree requirements after remove, then redistribute or merge them.
		int node_minimum_keys = ceil((float)(bpt->nkeys - 1) / 2);
		if (node->used >= node_minimum_keys){
			// delete is done, yay!
			return;
		} else {
			// redistribute or merge from adjacent node
			// first, look up the parent node and then find adjacent node and key
			int idx = bptree_node_ptr_index(bpt, node->parent, node);
			if (idx < node->parent->used){
				// look right sibling
				bptree_node_t *affected = node->parent->children[idx];
				bptree_node_redistribute_or_merge(bpt, affected, node, node, idx);
			} else {
				// pointer found in the most right of children
				// then look left sibling
				bptree_node_t *affected = node->parent->children[idx - 1];
				bptree_node_redistribute_or_merge(bpt, node, affected, node, idx - 1);
			}
		}
	}
}

void bptree_leaf_redistribute_or_merge(bptree_t *bpt, bptree_leaf_t *left_leaf, bptree_leaf_t *right_leaf, bptree_leaf_t *underfull_leaf, int parent_key_index){
	int leaf_minimum_keys = ceil((float)(bpt->nkeys - 1) / 2);
	if (left_leaf->used + right_leaf->used < bpt->nkeys){
		// merge
	} else {
		// redistribute pointers
		int need_keys = leaf_minimum_keys - underfull_leaf->used;
		if (need_keys <= 0){
			// nothing to do
			return;
		}
		if (underfull_leaf == left_leaf){
			// data moves from head of right leaf to end of left leaf
			for(int i = 0; i < need_keys; i++){
				left_leaf->keys[left_leaf->used] = right_leaf->keys[i];
				left_leaf->children[left_leaf->used] = right_leaf->children[i];
				right_leaf->keys[i] = right_leaf->keys[i + 1];
				right_leaf->children[i] = right_leaf->keys[i + 1];
				left_leaf->used += 1;
				right_leaf->used -= 1;
			}
		} else {
			// data moves from left to right
			for(int i = 0; i < need_keys; i++){
				right_leaf->keys[need_keys - i + 1] = right_leaf->keys[need_keys - i];
				right_leaf->children[need_keys - i + 1] = right_leaf->children[need_keys - i];
				right_leaf->keys[need_keys - i] = left_leaf->keys[left_leaf->used];
				right_leaf->children[need_keys - i] = left_leaf->children[left_leaf->used];
				left_leaf->used -= 1;
				right_leaf->used += 1;
			}
		}
		// finally update key in parent node
		right_leaf->parent->keys[parent_key_index] = right_leaf->keys[0];
	}
}

void bptree_leaf_delete(bptree_t *bpt, bptree_node_t *leaf, bptree_key_t key){
	int idx = bptree_node_key_index(bpt, leaf, key);
	if (idx < 0){
		// not found
		return;
	}
	// shift elements for deleting key on leaf
	for(int i = idx; i < leaf->used - 1; i++){
		leaf->keys[i] = leaf->keys[i + 1];
		leaf->children[i] = leaf->children[i + 1];
	}
	// keep last ptr on leaf
	// node->children[node->used - 1] = node->children[node->used];
	leaf->used -= 1;
	
	if (leaf == bpt->root){
		return;
	} else {
		// if leaf does not satisfy B+-tree requirements after remove, then redistribute or merge them.
		int node_minimum_keys = ceil((float)(bpt->nkeys - 1) / 2);
		if (leaf->used >= node_minimum_keys){
			// delete is done, yay!
			return;
		} else {
			// redistribute or merge from adjacent leaf
			// first, look up the parent node and then find adjacent leaf and key
			int idx = bptree_node_ptr_index(bpt, leaf->parent, leaf);
			if (idx < leaf->parent->used){
				// look right sibling
				bptree_node_t *affected = leaf->parent->children[idx];
				bptree_leaf_redistribute_or_merge(bpt, affected, leaf, leaf, idx);
			} else {
				// pointer found in the most right of children
				// then look left sibling
				bptree_node_t *affected = leaf->parent->children[idx - 1];
				bptree_leaf_redistribute_or_merge(bpt, leaf, affected, leaf, idx - 1);
			}
			return;
		}
	}
}


// search B+-tree passed by bpt
// returns child's pointer and status = 1 (if found)
// returns NULL and status = 0 (if not found)
void *bptree_search(bptree_t *bpt, bptree_key_t key, int *status){
	bptree_node_t *leaf = bptree_leaf_search(bpt, key);
	for (int i = 0; i < leaf->used; i++){
		if (key == leaf->keys[i]){
			*status = 1;
			return leaf->children[i];
		}
	}
	*status = 0;
	return NULL;
}


void bptree_insert(bptree_t *bpt, bptree_key_t key, void *value){
	bptree_node_t *leaf = bptree_leaf_search(bpt, key);
	bptree_leaf_insert(bpt, leaf, key, value);
}

void bptree_delete(bptree_t *bpt, bptree_key_t key){
	bptree_node_t *leaf = bptree_leaf_search(bpt, key);
	bptree_leaf_delete(bpt, leaf, key);
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

