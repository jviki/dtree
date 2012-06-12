CC = gcc
AR = ar
CFLAGS = -std=gnu99 -Wall -pedantic -Wextra -g -fPIC

Q ?= @

all: libdtree.a libdtree.so
libdtree.a: dtree_error.o dtree_procfs.o dtree.o bcd_arith.o
	$(Q) $(AR) rcs $@ $^

libdtree.so: dtree_error.o dtree_procfs.o dtree.o bcd_arith.o
	$(Q) $(CC) -shared -o $@ $^

busio: busio.o
	$(CC) $(LDFLAGS) $^ -L. -ldtree -o $@
busio.o: busio.c

clean:
	$(Q) $(RM) *.o

distclean: clean
	$(Q) $(RM) libdtree.a
	$(Q) $(RM) libdtree.so
	$(Q) $(RM) busio
