MODULE_big = pgspeck
OBJS = speck.o pgspeck.o

EXTENSION = pgspeck
DATA = pgspeck--1.0.sql

EXTRA_CLEAN = pgspeck-1.0.orig.tar.gz

REGRESS = pgspeck

ifdef NO_PGXS
subdir = contrib/pgspeck
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
else
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
endif

tarball:
	tar -czf pgspeck-1.0.orig.tar.gz                                            \
		META.json README.md                                                     \
		Makefile pgspeck--1.0.sql pgspeck.c pgspeck.control pgspeck.h speck.c   \
		expected sql

.PHONY: tarball
