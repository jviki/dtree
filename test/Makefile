CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -Wextra -I.. -g

Q ?= @

TESTS  = dtree_open_test
TESTS += dtree_next_test
TESTS += dtree_byname_test
TESTS += dtree_parsehex_test
TESTS += dtree_bycompat_test
TESTS += dtree_bcd_test

all: $(TESTS)
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
	$(Q) $(RM) $(TESTS)

force:
.PHONY: all clean force
