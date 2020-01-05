## libbptree
![](https://github.com/k5342/libbptree/workflows/test/badge.svg?dummy=unused)

### Interface
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
