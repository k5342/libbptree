AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = -std=c99 -lm -g -I$(top_srcdir)/include
TESTS = init node_create leaf_create leaf_insert insert_ascending insert_descending insert_random
check_PROGRAMS = init node_create leaf_create leaf_insert insert_ascending insert_descending insert_random

insert_ascending_SOURCES = insert_ascending.c testutils.c
insert_ascending_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

insert_descending_SOURCES = insert_descending.c testutils.c
insert_descending_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

insert_random_SOURCES = insert_random.c testutils.c
insert_random_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

init_SOURCES = init.c testutils.c
init_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

node_create_SOURCES = node_create.c testutils.c
node_create_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

leaf_create_SOURCES = leaf_create.c testutils.c
leaf_create_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

leaf_insert_SOURCES = leaf_insert.c testutils.c
leaf_insert_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree
