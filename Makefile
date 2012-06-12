CC = gcc
AR = ar
CFLAGS = -std=gnu99 -Wall -pedantic -Wextra -g

Q ?= @

all: libdtree.a
libdtree.a: dtree_error.o dtree_procfs.o dtree.o bcd_arith.o
	$(Q) $(AR) rcs $@ $^

busio: busio.o libdtree.a
busio.o: busio.c

lua-test:
	$(CC) -o lua-test -DTEST lua_dtree.c -llua -L. -ldtree

clean:
	$(Q) $(RM) *.o
