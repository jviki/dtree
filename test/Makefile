CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -Wextra -I..

Q ?= @
LDFLAGS = -L.. -ldtree

all: dtree_open_test
dtree_open_test: dtree_open_test.o

clean:
	$(Q) $(RM) *.o
