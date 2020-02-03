#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

typedef int64_t bptree_key_t;

typedef struct bptree_node {
	struct bptree *context;
	struct bptree_node *parent;
	struct bptree_node **children;
	bptree_key_t *keys;
	int used;
	int is_leaf;
} bptree_node_t;

typedef struct bptree {
	int nkeys;
	struct bptree_node *root;
} bptree_t;

// libbptree interface
bptree_t *bptree_init(int nkeys);
void bptree__free(bptree_t *bpt, bptree_node_t *node);
void bptree_free(bptree_t *bpt);
void bptree_insert(bptree_t *bpt, bptree_key_t key, void *value);
void bptree_delete(bptree_t *bpt, bptree_key_t key);
int bptree_search(bptree_t *bpt, bptree_key_t key, void **ptr);

int bptree_node_get_key_count(bptree_t *bpt, bptree_node_t *node);
bptree_key_t bptree_node_get_key_by_index(bptree_t *bpt, bptree_node_t *node, int index);
bptree_node_t *bptree_node_get_children_by_index(bptree_t *bpt, bptree_node_t *node, int index);
int bptree_node_is_leaf(bptree_t *bpt, bptree_node_t *node);
void bptree_node_delete(bptree_t *bpt, bptree_node_t *node, bptree_node_t *ptr);

int bptree_leaf_get_key_count(bptree_t *bpt, bptree_node_t *leaf);
bptree_key_t bptree_leaf_get_key_by_index(bptree_t *bpt, bptree_node_t *leaf, int index);
void *bptree_leaf_get_element_by_index(bptree_t *bpt, bptree_node_t *leaf, int index);
void bptree_leaf_delete(bptree_t *bpt, bptree_node_t *leaf, bptree_key_t key);

// helpers
void bptree_leaf_insert(bptree_t *bpt, bptree_node_t *leaf, bptree_key_t key, void *value);
bptree_node_t *bptree_leaf_search(bptree_t *bpt, bptree_key_t key);
void bptree_node_insert(bptree_t *bpt, bptree_node_t *l_child, bptree_key_t key, bptree_node_t *r_child);
int bptree_node_insert_index(bptree_t *bpt, bptree_node_t *node, bptree_key_t key);
void bptree_leaf_redistribute_or_merge(bptree_t *bpt, bptree_node_t *left_leaf, bptree_node_t *right_leaf, bptree_node_t *underfull_leaf, int parent_key_index);

bptree_node_t *bptree_node_create(bptree_t *bpt);
bptree_node_t *bptree_leaf_create(bptree_t *bpt);
void bptree_node_destroy(bptree_node_t *node);

bptree_node_t *bptree_leaf_get_leftmost(bptree_t *bpt);
bptree_node_t *bptree_leaf_get_rightmost(bptree_t *bpt);
bptree_node_t *bptree_leaf_get_rightadjacent(bptree_t *bpt, bptree_node_t *leaf);

// utils
void bptree_print(bptree_t *bpt);
void bptree_node_print(bptree_t *bpt, bptree_node_t *node);
void bptree_leaf_print(bptree_t *bpt, bptree_node_t *leaf);
void bptree_perror(char *s);

#endif
