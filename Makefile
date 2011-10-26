CC = gcc
AR = ar
CFLAGS = -std=c99 -Wall -pedantic -Wextra -g

Q ?= @

all: libdtree.a
libdtree.a: dtree_error.o dtree_procfs.o dtree.o
	$(Q) $(AR) rcs $@ $^

clean:
	$(Q) $(RM) *.o
