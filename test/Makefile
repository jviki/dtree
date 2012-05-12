CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -Wextra -I.. -g

Q ?= @

all: dtree_open_test dtree_next_test dtree_byname_test dtree_parsehex_test dtree_bycompat_test dtree_bcd_test
dtree_open_test: dtree_open_test.o libdtree.a
dtree_next_test: dtree_next_test.o libdtree.a
dtree_byname_test: dtree_byname_test.o libdtree.a
dtree_parsehex_test: dtree_parsehex_test.c libdtree.a
dtree_bycompat_test: dtree_bycompat_test.c libdtree.a
dtree_bcd_test: dtree_bcd_test.c libdtree.a

libdtree.a: force
	$(Q) $(MAKE) -C .. $@
	$(Q) ln -f ../$@ $@

clean:
	$(Q) $(RM) *.o

force:
.PHONY: all clean force
