## libbptree
![](https://github.com/k5342/libbptree/workflows/test/badge.svg?dummy=unused)

The B+-tree is a data structure which is intended to store pairs of a _key_ and _value_.  The _key_ is a **unique** integer to distinguish _value_ in B+-tree.  The _value_ is data to be stored in B+-tree and associated with a _key_.  The libbptree manages _key_ and _value_ as a following types; `bptree_key_t` and `void *`, respectively.

### Types
- **bptree_t** - Holds a header and pointer to the body of B+-tree. It is an alias to `struct bptree`.
- **bptree_key_t** - Represents the _key_ of B+-tree. It is an alias to `long long`.

### Interfaces
```c
bptree_t *bptree_init(int nkeys);
void bptree_free(bptree_t *bpt);
void bptree_insert(bptree_t *bpt, bptree_key_t key, void *value);
int bptree_search(bptree_t *bpt, bptree_key_t key, void *value);
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
- [ ] Delete
- [ ] Write unit tests (WIP)
- [ ] Makefile
- [ ] Refactoring
