## libbptree
![](https://github.com/k5342/libbptree/workflows/test/badge.svg)

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
- [ ] Write unit tests
- [ ] Makefile
- [ ] Refactoring
