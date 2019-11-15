AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = -std=c99 -lm -g -I$(top_srcdir)/include
TESTS = insert
check_PROGRAMS = insert
insert_SOURCES = insert.c testutils.c
insert_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree

