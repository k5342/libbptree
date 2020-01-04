AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = -std=c99 -lm -g -I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

check_PROGRAMS = \
		init \
		node_create \
		leaf_create \
		leaf_insert \
		insert_ascending \
		insert_descending \
		insert_random
TESTS = $(check_PROGRAMS)

init_SOURCES = init.c testutils.c
node_create_SOURCES = node_create.c testutils.c
leaf_create_SOURCES = leaf_create.c testutils.c
leaf_insert_SOURCES = leaf_insert.c testutils.c
insert_ascending_SOURCES = insert_ascending.c testutils.c
insert_descending_SOURCES = insert_descending.c testutils.c
insert_random_SOURCES = insert_random.c testutils.c

