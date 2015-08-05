MODULE_big = pgspeck
OBJS = speck.o pgspeck.o

EXTENSION = pgspeck
DATA = pgspeck--1.0.sql

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
