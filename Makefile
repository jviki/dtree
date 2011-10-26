CC = gcc
AR = ar
CFLAGS = -std=c99 -Wall -pedantic -Wextra

all: libdtree.a
libdtree.a: dtree_error.o dtree_procfs.o dtree.o

clean:
	$(RM) *.o
