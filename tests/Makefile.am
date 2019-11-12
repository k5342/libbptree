AM_CFLAGS = -std=c99 -lm -g -I$(top_srcdir)/include
noinst_PROGRAMS = insert verify
insert_SOURCES = insert.c
insert_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree
verify_SOURCES = verify.c
verify_LDFLAGS = -L$(top_srcdir)/src/.libs -lbptree
