CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -Wextra -I.. -g

CFLAGS += -DDEVICE_TREE='"/proc/device-tree"'

Q ?= @
VALGRIND ?= valgrind --leak-check=full --show-reachable=yes

TESTS  = dtree_open_test
TESTS += dtree_next_test
TESTS += dtree_byname_test
TESTS += dtree_parsehex_test
TESTS += dtree_bycompat_test
TESTS += dtree_bcd_test
TESTS += dtree_procfs_llist_test
TESTS += dtree_stack_test

all: $(TESTS)
dtree_open_test: dtree_open_test.o libdtree.a
dtree_next_test: dtree_next_test.o libdtree.a
dtree_byname_test: dtree_byname_test.o libdtree.a
dtree_parsehex_test: dtree_parsehex_test.c libdtree.a
dtree_bycompat_test: dtree_bycompat_test.c libdtree.a
dtree_bcd_test: dtree_bcd_test.c libdtree.a
dtree_procfs_llist_test: dtree_procfs_llist_test.c ../dtree_error.c ../bcd_arith.c ../dtree.c
dtree_stack_test: dtree_stack_test.c ../dtree_error.c

ifeq ($(SHELL),/bin/bash)
run: run-bash
else
run: $(TESTS)
	$(Q) for test in $(TESTS); do $(VALGRIND) ./$$test; done
endif

run-bash: $(TESTS)
	$(Q) fail=$$(tput bold; tput setaf 1) &&           \
	     pass=$$(tput bold; tput setaf 2) &&           \
	     normal=$$(tput sgr0)                          \
	  && for test in $(TESTS); do $(VALGRIND) ./$$test 2>&1 | sed  \
	     -e "s/ERROR/$${fail}ERROR$${normal}/"         \
	     -e "s/SUCCESS/$${pass}SUCCESS$${normal}/"; done

libdtree.a: force
	$(Q) $(MAKE) -C .. $@
	$(Q) ln -f ../$@ $@

clean:
	$(Q) $(RM) *.o
	$(Q) $(RM) $(TESTS)

force:
.PHONY: all clean force
