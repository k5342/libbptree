#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef long long bptree_key_t;

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
void bptree_free(bptree_t *bpt);
void bptree_insert(bptree_t *bpt, bptree_key_t key, void *value);
int bptree_search(bptree_t *bpt, bptree_key_t key, void **ptr);

// helpers
void bptree_leaf_insert(bptree_t *bpt, bptree_node_t *leaf, bptree_key_t key, void *value);
bptree_node_t *bptree_leaf_search(bptree_t *bpt, bptree_key_t key);
void bptree_node_insert(bptree_t *bpt, bptree_node_t *l_child, bptree_key_t key, bptree_node_t *r_child);
int bptree_node_insert_index(bptree_t *bpt, bptree_node_t *node, bptree_key_t key);

bptree_node_t *bptree_node_create(bptree_t *bpt);
bptree_node_t *bptree_leaf_create(bptree_t *bpt);
void bptree_node_destroy(bptree_node_t *node);

// utils
void bptree_print(bptree_t *bpt);
void bptree_node_print(bptree_t *bpt, bptree_node_t *node);
void bptree_leaf_print(bptree_t *bpt, bptree_node_t *leaf);
void bptree_perror(char *s);

#endif
