CC = gcc
AR = ar
CFLAGS = -std=c99 -Wall -pedantic -Wextra

Q ?= @

all: libdtree.a
libdtree.a: dtree_error.o dtree_procfs.o dtree.o

clean:
	$(Q) $(RM) *.o
