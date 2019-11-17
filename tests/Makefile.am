AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = -std=c99 -lm -g -I$(top_srcdir)/include
TESTS = init node_create insert
check_PROGRAMS = init node_create insert
insert_SOURCES = insert.c testutils.c
insert_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

init_SOURCES = init.c testutils.c
init_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

node_create_SOURCES = node_create.c testutils.c
node_create_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree
