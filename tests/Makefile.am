AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = -std=c99 -lm -g -I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

check_PROGRAMS = \
		init \
		node_create \
		leaf_create \
		leaf_insert \
		leaf_delete \
		leaf_borrow_keys \
		node_borrow_keys \
		node_shift \
		insert_ascending \
		insert_descending \
		insert_random \
		insert_ascending_odd \
		insert_descending_odd \
		insert_random_odd \
		insert_ascending_small \
		insert_descending_small \
		insert_random_small \
		delete_ascending \
		delete_descending \
		delete_random \
		delete_ascending_odd \
		delete_descending_odd \
		delete_random_odd \
		delete_ascending_small \
		delete_descending_small \
		delete_random_small
TESTS = $(check_PROGRAMS)

init_SOURCES = init.c testutils.c
node_create_SOURCES = node_create.c testutils.c
leaf_create_SOURCES = leaf_create.c testutils.c
leaf_insert_SOURCES = leaf_insert.c testutils.c
leaf_delete_SOURCES = leaf_delete.c testutils.c
leaf_borrow_keys_SOURCES = leaf_borrow_keys.c testutils.c
node_borrow_keys_SOURCES = node_borrow_keys.c testutils.c
node_shift_SOURCES = node_shift.c testutils.c
insert_ascending_SOURCES = insert_ascending.c testutils.c
insert_descending_SOURCES = insert_descending.c testutils.c
insert_random_SOURCES = insert_random.c testutils.c
insert_ascending_small_SOURCES = insert_ascending_small.c testutils.c
insert_descending_small_SOURCES = insert_descending_small.c testutils.c
insert_random_small_SOURCES = insert_random_small.c testutils.c
insert_ascending_odd_SOURCES = insert_ascending_odd.c testutils.c
insert_descending_odd_SOURCES = insert_descending_odd.c testutils.c
insert_random_odd_SOURCES = insert_random_odd.c testutils.c
delete_ascending_SOURCES = delete_ascending.c testutils.c
delete_descending_SOURCES = delete_descending.c testutils.c
delete_random_SOURCES = delete_random.c testutils.c
delete_ascending_small_SOURCES = delete_ascending_small.c testutils.c
delete_descending_small_SOURCES = delete_descending_small.c testutils.c
delete_random_small_SOURCES = delete_random_small.c testutils.c
delete_ascending_odd_SOURCES = delete_ascending_odd.c testutils.c
delete_descending_odd_SOURCES = delete_descending_odd.c testutils.c
delete_random_odd_SOURCES = delete_random_odd.c testutils.c
