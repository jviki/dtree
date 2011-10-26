CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -Wextra -I..

Q ?= @

all: dtree_open_test
dtree_open_test: dtree_open_test.o libdtree.a
libdtree.a:
	$(Q) $(MAKE) -C .. $@
	$(Q) ln ../$@ $@

clean:
	$(Q) $(RM) *.o
