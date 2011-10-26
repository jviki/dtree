CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -Wextra -I..

Q ?= @

all: dtree_open_test dtree_next_test
dtree_open_test: dtree_open_test.o libdtree.a
dtree_next_test: dtree_next_test.o libdtree.a

libdtree.a: force
	$(Q) $(MAKE) -C .. $@
	$(Q) ln -f ../$@ $@

clean:
	$(Q) $(RM) *.o

force:
.PHONY: all clean force
