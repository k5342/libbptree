AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = -std=c99 -lm -g -I$(top_srcdir)/include
TESTS = init insert
check_PROGRAMS = init insert
insert_SOURCES = insert.c testutils.c
insert_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

init_SOURCES = init.c testutils.c
init_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree
