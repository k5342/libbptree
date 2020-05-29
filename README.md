## libbptree

![](https://github.com/k5342/libbptree/workflows/test/badge.svg)
![](https://github.com/k5342/libbptree/workflows/test/sanitizer/badge.svg)

An implementation of [B+-tree](https://en.wikipedia.org/wiki/B%2B_tree).  The B+-tree is a data structure which is intended to store pairs of a _key_ and _value_.  The _key_ is a **unique** integer to distinguish _value_ in B+-tree.  The _value_ is data to be stored in B+-tree and associated with a _key_.  The libbptree manages _key_ and _value_ as a following types; `bptree_key_t` and `void *`, respectively.

### Types
- **bptree_t** - Holds a header and pointer to the body of B+-tree. It is an alias of `struct bptree`.
- **bptree_key_t** - Represents the _key_ of B+-tree. It is an alias of `int64_t`.
- **bptree_node_t** - Represents the *internal node* and *leaf* in B+-tree. It is an alias of `struct bptree_node`. Elements in `bptree_node_t` can be accessed via *accessor functions* shown below.

### Interfaces
```c
bptree_t *bptree_init(int nkeys);
void bptree_free(bptree_t *bpt);
void bptree_insert(bptree_t *bpt, bptree_key_t key, void *value);
int bptree_search(bptree_t *bpt, bptree_key_t key, void **value);
void bptree_node_delete(bptree_t *bpt, bptree_node_t *node, bptree_node_t *ptr);
```
Note: These functions currently does not support multithreading.

#### Accessors for nodes
```c
int bptree_node_get_key_count(bptree_t *bpt, bptree_node_t *node);
bptree_key_t bptree_node_get_key_by_index(bptree_t *bpt, bptree_node_t *node, int index);
bptree_node_t *bptree_node_get_children_by_index(bptree_t *bpt, bptree_node_t *node, int index);
int bptree_node_is_leaf(bptree_t *bpt, bptree_node_t *node);
```

#### Accessors for leaves
```c
int bptree_leaf_get_key_count(bptree_t *bpt, bptree_node_t *leaf);
bptree_key_t bptree_leaf_get_key_by_index(bptree_t *bpt, bptree_node_t *leaf, int index);
void *bptree_leaf_get_element_by_index(bptree_t *bpt, bptree_node_t *leaf, int index);
bptree_node_t *bptree_leaf_get_rightadjacent(bptree_t *bpt, bptree_node_t *leaf);
```

### Requirements
- -std=c99 support
- Autotools
- libtool

#### On Ubuntu
```
# apt-get install gcc autoconf libtool
```

### Build
```
git clone ...
autoreconf -i -f
./configure --prefix=/path/to/your/install/dir
make
make install
```

### TODO
- [x] Insert
- [x] Delete
- [ ] Write unit tests (WIP)
- [ ] Makefile
- [ ] Refactoring
- [ ] Thread-Safe
