#include <bptree.h>

void bptree_perror(char *s){
	if (s == NULL){
		perror("bptree_perror: s is NULL\n");
		return;
	}
	fprintf(stderr, "bptree: %s\n", s);
}

void bptree_leaf_print(bptree_t *bpt, bptree_node_t *leaf){
	printf("{");
	if (leaf->used > 0){
		for(int i = 0; i < leaf->used; i++){
			printf("%p %lld", leaf->children[i], leaf->keys[i]);
			if (i < leaf->used - 1){
				printf(" ");
			}
		}
	}
	printf("|%p", leaf->children[bpt->nkeys]);
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
#ifdef DEBUG
	printf("bpt->root = %p\n", bpt->root);
#endif
	bptree_node_print(bpt, bpt->root);
	printf("\n");
}

bptree_node_t *bptree_node_create(bptree_t *bpt){
	if (bpt == NULL){
		bptree_perror("bptree_node_create: bpt is NULL");
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
#ifdef DEBUG
	printf("bptree_node_destroy(node = %p)\n", node);
#endif
	if (node == NULL){
		bptree_perror("bptree_node_destroy: node is NULL");
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
#ifdef DEBUG
	printf("bptree_node_ptr_index(bpt = %p, node = %p, ptr = %p)\n", bpt, node, ptr);
	printf("bptree_node_ptr_index: node->used = %d\n", node->used);
#endif
	for(int i = 0; i <= node->used; i++){
#ifdef DEBUG
		printf("bptree_node_ptr_index: node->children[%d] = %p\n", i, node->children[i]);
#endif
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
	printf("bptree_node_insert: bpt = %p, l_child = %p, key = %lld, r_child = %p\n", bpt, l_child, key, r_child);
	printf("bptree_node_insert: l_child = ");
	bptree_node_print(bpt, l_child);
	printf("\n");
	printf("bptree_node_insert: r_child = ");
	bptree_node_print(bpt, r_child);
	printf("\n");
#endif
	bptree_node_t *dst_node = l_child->parent;
	if (dst_node == NULL){
		// divide root
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
	printf("bptree_node_insert: insert_index = %d, r_child = %p\n", insert_index, r_child);
	printf("bptree_node_insert: r_child = ");
	bptree_node_print(bpt, r_child);
	printf("\n");
#endif
	if (dst_node->used + 1 <= bpt->nkeys){
		// shift and insert
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
		printf("bptree_node_insert: divide node\n");
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
			if (current->keys[idx] > key){
				break;
			}
		}
		current = current->children[idx];
	}
	return current;
}

void bptree_leaf_insert(bptree_t *bpt, bptree_node_t *leaf, bptree_key_t key, void *value){
#ifdef DEBUG
	printf("bptree_leaf_insert(bpt = %p, leaf = %p, key = %lld, value = %p)\n", bpt, leaf, key, value);
	printf("bptree_leaf_insert: leaf = ");
	bptree_leaf_print(bpt, leaf);
	printf("\n");
#endif
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
		printf("bptree_leaf_insert: divide leaf\n");
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
		new->children[bpt->nkeys] = leaf->children[bpt->nkeys];
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
void bptree_node_delete(bptree_t *bpt, bptree_node_t *node, bptree_node_t *ptr){
#ifdef DEBUG
	printf("bptree_node_delete(bpt: %p, node: %p, ptr: %p)\n", bpt, node, ptr);
	printf("bptree_node_delete: node = ");
	bptree_leaf_print(bpt, node);
	printf("\n");
	printf("bptree_node_delete: ptr = ");
	bptree_leaf_print(bpt, ptr);
	printf("\n");
#endif
	int idx_ptr = bptree_node_ptr_index(bpt, node, ptr);
#ifdef DEBUG
	printf("bptree_node_delete: bptree_node_ptr_index(bpt: %p, node: %p, ptr: %p) = %d\n", bpt, node, ptr, idx_ptr);
#endif
	int idx_key;
	if (idx_ptr < 0){
		// not found
		return;
	} else if (idx_ptr == 0){
		idx_key = 0;
	} else {
		idx_key = idx_ptr - 1;
	}
	
	// shift elements on node to remove ptr
	for(int i = idx_ptr; i < node->used; i++){
		node->children[i] = node->children[i + 1];
	}
	
	// shift elements on node to remove corresponding key
	for(int i = idx_key; i < node->used - 1; i++){
		node->keys[i] = node->keys[i + 1];
	}
	
	node->used -= 1;
	
#ifdef DEBUG
	printf("bptree_node_delete: delete complete for %p\n", node);
	printf("bptree_node_delete: node = ");
	bptree_leaf_print(bpt, node);
	printf("\n");
#endif
	
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
			bptree_leaf_print(bpt, node->parent);
			printf("\n");
			int idx = bptree_node_ptr_index(bpt, node->parent, node);
			if (idx < node->parent->used){
				// look right sibling
				bptree_node_t *affected = node->parent->children[idx + 1];
				bptree_node_redistribute_or_merge(bpt, node, affected, node, idx);
			} else {
				// pointer found in the most right of children
				// then look left sibling
				bptree_node_t *affected = node->parent->children[idx - 1];
				bptree_node_redistribute_or_merge(bpt, affected, node, node, idx - 1);
			}
		}
	}
}

void bptree_node_redistribute_or_merge(bptree_t *bpt, bptree_node_t *left_node, bptree_node_t *right_node, bptree_node_t *underfull_node, int parent_key_index){
#ifdef DEBUG
	printf("bptree_node_redistribute_or_merge(bpt = %p, left_node = %p, right_node = %p, underfull_node = %p, parent_key_index = %d)\n", bpt, left_node, right_node, underfull_node, parent_key_index);
	printf("bptree_node_redistribute_or_merge: left_node->used = %d, right_node->used = %d\n", left_node->used, right_node->used);
	printf("left:\n");
	bptree_node_print(bpt, left_node);
	printf("\n");
	printf("right:\n");
	bptree_node_print(bpt, right_node);
	printf("\n");
#endif
	int node_minimum_keys = ceil((float)bpt->nkeys / 2);
	bptree_node_t *borrow_node = (left_node == underfull_node) ? right_node : left_node;
	if (borrow_node->used - 1 < node_minimum_keys){
		// merge required
		// first, pull down an appropriate key from parent node
		bptree_key_t pulldown_key = left_node->parent->keys[parent_key_index];
		left_node->keys[left_node->used++] = pulldown_key;
		
		// second, move data from immediately adjecent node (do merge)
		// left_node <--- right_node
		for(int i = 0; i < right_node->used; i++){
#ifdef DEBUG
			printf("left_node->used = %d, right_node->used = %d, i = %d\n", left_node->used, right_node->used, i);
			bptree_leaf_print(bpt, right_node->children[i]);
			printf("\n");
#endif
			right_node->children[i]->parent = left_node;
			left_node->keys[left_node->used] = right_node->keys[i];
			left_node->children[left_node->used] = right_node->children[i];
			left_node->used += 1;
		}
#ifdef DEBUG
		bptree_leaf_print(bpt, right_node->children[right_node->used]);
		printf("\n");
#endif
		left_node->children[left_node->used] = right_node->children[right_node->used];
		left_node->children[left_node->used]->parent = left_node;
		
		bptree_node_delete(bpt, left_node->parent, right_node);
		
		// if the root node becomes empty, then free it
		if (left_node->parent == bpt->root){
#ifdef DEBUG
			printf("bptree_node_redistribute_or_merge: bpt->root->used = %d\n", bpt->root->used);
#endif
			if (bpt->root->used == 0){
#ifdef DEBUG
				printf("bptree_node_redistribute_or_merge: destroy old root\n");
#endif
				bptree_node_destroy(bpt->root);
				bpt->root = left_node;
				bpt->root->parent = NULL;
			}
		}
		
		bptree_node_destroy(right_node);
	} else {
		// redistribute pointers
		int need_keys = node_minimum_keys - underfull_node->used;
		printf("need_keys: %d\n", need_keys);
		bptree_node_borrow_keys(bpt, left_node, right_node, underfull_node, need_keys, parent_key_index);
	}
}

void bptree_node_borrow_keys(bptree_t *bpt, bptree_node_t *left_node, bptree_node_t *right_node,
								bptree_node_t *underfull_node, int need_keys, int parent_key_index){
	if (need_keys <= 0){
		// nothing to do
		return;
	}
	if (underfull_node == left_node){
		// append left_node->children from right_node->children[0]
		left_node->keys[left_node->used++] = left_node->parent->keys[parent_key_index];
		left_node->children[left_node->used] = right_node->children[0];
		left_node->children[left_node->used]->parent = left_node;
		
		// update key in parent node
		left_node->parent->keys[parent_key_index] = right_node->keys[need_keys - 1];
		
		// data moves from head of right node to end of left node
		for(int i = 1; i < need_keys; i++){
			left_node->keys[left_node->used++] = right_node->keys[i - 1];
			left_node->children[left_node->used] = right_node->children[i];
			left_node->children[left_node->used]->parent = left_node;
		}
		
		// shift data {need_keys} times on right_node
		bptree_node_shift(bpt, right_node, -need_keys);
		right_node->used -= need_keys;
	} else {
		// shift data
		bptree_node_shift(bpt, right_node, need_keys);
		
		// data moves from left to right
		for(int i = 0; i < need_keys - 1; i++){
			right_node->keys[i] = left_node->keys[left_node->used - 1];
			right_node->children[i] = left_node->children[left_node->used - 1];
			right_node->children[i]->parent = right_node;
			left_node->used -= 1;
		}
		
		right_node->keys[need_keys - 1] = right_node->parent->keys[parent_key_index];
		right_node->children[need_keys - 1] = left_node->children[left_node->used];
		right_node->children[need_keys - 1]->parent = right_node;
		right_node += need_keys;
		
		// finally update key in parent node
		right_node->parent->keys[parent_key_index] = right_node->keys[0];
	}
}

void bptree_leaf_redistribute_or_merge(bptree_t *bpt, bptree_node_t *left_leaf, bptree_node_t *right_leaf, bptree_node_t *underfull_leaf, int parent_key_index){
#ifdef DEBUG
	printf("bptree_leaf_redistribute_or_merge(bpt = %p, left_leaf = %p, right_leaf = %p, underfull_leaf = %p, parent_key_index = %d)\n", bpt, left_leaf, right_leaf, underfull_leaf, parent_key_index);
	printf("bptree_leaf_redistribute_or_merge: left_leaf->used = %d, right_leaf->used = %d\n", left_leaf->used, right_leaf->used);
	printf("left:\n");
	bptree_leaf_print(bpt, left_leaf);
	printf("\n");
	printf("right:\n");
	bptree_leaf_print(bpt, right_leaf);
	printf("\n");
#endif
	int leaf_minimum_keys = ceil((float)bpt->nkeys / 2);
	bptree_node_t *borrow_leaf = (left_leaf == underfull_leaf) ? right_leaf : left_leaf;
	printf("borrow_leaf: %p, used = %d\n", borrow_leaf, borrow_leaf->used);
	if (borrow_leaf->used - 1 < leaf_minimum_keys){
		// merge
#ifdef DEBUG
		printf("bptree_leaf_redistribute_or_merge: merge\n");
#endif
		for(int i = 0; i < right_leaf->used; i++){
			printf("left_leaf->used = %d, right_leaf->used = %d, i = %d\n", left_leaf->used, right_leaf->used, i);
			left_leaf->keys[left_leaf->used] = right_leaf->keys[i];
			left_leaf->children[left_leaf->used] = right_leaf->children[i];
			left_leaf->used += 1;
		}
		left_leaf->children[bpt->nkeys] = right_leaf->children[bpt->nkeys];
		
		// propagate to parent node
		bptree_node_delete(bpt, left_leaf->parent, right_leaf);
		
		// if the root node becomes empty, then free it
		if (left_leaf->parent == bpt->root){
#ifdef DEBUG
			printf("bptree_leaf_redistribute_or_merge: bpt->root->used = %d\n", bpt->root->used);
#endif
			if (bpt->root->used == 0){
#ifdef DEBUG
				printf("bptree_leaf_redistribute_or_merge: destroy old root\n");
#endif
				bptree_node_destroy(bpt->root);
				bpt->root = left_leaf;
				bpt->root->parent = NULL;
				bpt->root->children[bpt->nkeys] = NULL;
			}
		}
		
		// free merged node
#ifdef DEBUG
		printf("bptree_leaf_redistribute_or_merge: destroy right_leaf = ");
		bptree_node_print(bpt, right_leaf);
		printf("\n");
#endif
		bptree_node_destroy(right_leaf);
		return;
	} else {
		// redistribute pointers
		int need_keys = leaf_minimum_keys - underfull_leaf->used;
		printf("need_keys: %d\n", need_keys);
		bptree_leaf_borrow_keys(bpt, left_leaf, right_leaf, underfull_leaf, need_keys, parent_key_index);
	}
}

void bptree_leaf_borrow_keys(bptree_t *bpt, bptree_node_t *left_leaf, bptree_node_t *right_leaf,
								bptree_node_t *underfull_leaf, int need_keys, int parent_key_index){
	if (need_keys <= 0){
		// nothing to do
		return;
	}
	if (underfull_leaf == left_leaf){
		// left_leaf: append data
		// right_leaf: left shift data
		for(int i = 0; i < need_keys; i++){
			left_leaf->keys[left_leaf->used] = right_leaf->keys[i];
			left_leaf->children[left_leaf->used] = right_leaf->children[i];
			left_leaf->used += 1;
		}
		bptree_node_shift(bpt, right_leaf, -need_keys);
		right_leaf->used -= need_keys;
	} else {
		// right_leaf: shift data + insert data
		// left_leaf: decrement key count
		bptree_node_shift(bpt, right_leaf, need_keys);
		
		// insert data
		for(int i = 0; i < need_keys; i++){
			right_leaf->keys[i] = left_leaf->keys[left_leaf->used - 1];
			right_leaf->children[i] = left_leaf->children[left_leaf->used - 1];
			left_leaf->used -= 1;
			right_leaf->used += 1;
		}
	}
	// finally update key in parent node
	right_leaf->parent->keys[parent_key_index] = right_leaf->keys[0];
}

// shift key and pointers in node, not modify node->used
// n < 0: left shift
// n > 0: right shift
// TODO: merge shift on insert
void bptree_node_shift(bptree_t *bpt, bptree_node_t *node, int n){
	if (n == 0){
		return;
	}
	if (n < 0){
		n = -n;
		for(int i = 0; i < node->used - n; i++){
			node->keys[i] = node->keys[i + n];
			node->children[i] = node->children[i + n];
		}
		if (node->is_leaf == false){
			node->children[node->used - n] = node->children[node->used];
		}
	} else {
		if (node->is_leaf == false){
			node->children[node->used + n] = node->children[node->used];
		}
		for(int i = node->used + n - 1; i >= n; i--){
			node->keys[i] = node->keys[i - n];
			node->children[i] = node->children[i - n];
		}
	}
}

void bptree_leaf_delete(bptree_t *bpt, bptree_node_t *leaf, bptree_key_t key){
#ifdef DEBUG
	printf("bptree_leaf_delete(bpt: %p, leaf: %p, key: %lld)\n", bpt, leaf, key);
	printf("bptree_leaf_delete: leaf = ");
	bptree_leaf_print(bpt, leaf);
	printf("\n");
#endif
	int idx = bptree_node_key_index(bpt, leaf, key);
#ifdef DEBUG
	printf("bptree_leaf_delete: bptree_node_key_index(bpt: %p, leaf: %p, key: %lld) = %d\n", bpt, leaf, key, idx);
#endif
	if (idx < 0){
		// not found
		return;
	}
	// shift elements for deleting key on leaf
	for(int i = idx; i < leaf->used - 1; i++){
		leaf->keys[i] = leaf->keys[i + 1];
		leaf->children[i] = leaf->children[i + 1];
	}
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
#ifdef DEBUG
			printf("bptree_leaf_delete: merge or redistribute needed.\n");
			printf("bptree_leaf_delete: leaf->parent->used = %d, idx of ptr on parent = %d\n", leaf->parent->used, idx);
#endif
			if (idx < leaf->parent->used){
				printf("-------------\n");
				// look right sibling
				bptree_node_t *affected = leaf->parent->children[idx + 1];
				bptree_leaf_redistribute_or_merge(bpt, leaf, affected, leaf, idx);
			} else {
				// pointer found in the most right of children
				// then look left sibling
				printf("##########\n");
				printf("right\n");
				bptree_node_t *affected = leaf->parent->children[idx - 1];
				bptree_leaf_redistribute_or_merge(bpt, affected, leaf, leaf, idx - 1);
			}
			return;
		}
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

int bptree_node_get_key_count(bptree_t *bpt, bptree_node_t *node){
	return node->used;
}

bptree_key_t bptree_node_get_key_by_index(bptree_t *bpt, bptree_node_t *node, int index){
	if (index < 0){
		index += node->used;
	}
	return node->keys[index];
}

bptree_node_t *bptree_node_get_children_by_index(bptree_t *bpt, bptree_node_t *node, int index){
	if (index < 0){
		index += node->used;
	}
	return node->children[index];
}

int bptree_node_is_leaf(bptree_t *bpt, bptree_node_t *node){
	return node->is_leaf;
}

int bptree_leaf_get_key_count(bptree_t *bpt, bptree_node_t *leaf){
	return leaf->used;
}

bptree_key_t bptree_leaf_get_key_by_index(bptree_t *bpt, bptree_node_t *leaf, int index){
	if (index < 0){
		index += leaf->used;
	}
	return leaf->keys[index];
}

void *bptree_leaf_get_element_by_index(bptree_t *bpt, bptree_node_t *leaf, int index){
	if (index < 0){
		index += leaf->used;
	}
	return (void *)leaf->children[index];
}

bptree_node_t *bptree_leaf_get_leftmost(bptree_t *bpt){
	bptree_node_t *current = bpt->root;
	while (!current->is_leaf){
		current = current->children[0];
	}
	return current;
}

bptree_node_t *bptree_leaf_get_rightmost(bptree_t *bpt){
	bptree_node_t *current = bpt->root;
	while (!current->is_leaf){
		current = current->children[current->used];
	}
	return current;
}

bptree_node_t *bptree_leaf_get_rightadjacent(bptree_t *bpt, bptree_node_t *leaf){
	return leaf->children[bpt->nkeys];
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
		bptree_perror("bptree_init: nkeys must be bigger than zero");
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

void bptree__free(bptree_t *bpt, bptree_node_t *node){
	// this is for internal
	if (node->is_leaf == false){
		for (int i = 0; i < node->used + 1; i++){
			bptree__free(bpt, node->children[i]);
		}
	}
	bptree_node_destroy(node);
	return;
}

void bptree_free(bptree_t *bpt){
	if (bpt == NULL){
		bptree_perror("bptree_free: bpr is NULL");
	}
	bptree__free(bpt, bpt->root);
	free(bpt);
}

