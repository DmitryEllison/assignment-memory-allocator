CFLAGS=--std=c17 -Wall -pedantic -Isrc/ -ggdb -Wextra -Werror -DDEBUG
BUILDDIR=build
SRCDIR=src
TESTIKDIR=test
CC=gcc

all: $(BUILDDIR)/mem.o $(BUILDDIR)/util.o $(BUILDDIR)/mem_debug.o $(BUILDDIR)/testik.o $(BUILDDIR)/main.o
	$(CC) -o $(BUILDDIR)/main $^

build:
	mkdir -p $(BUILDDIR)

# check makefile -------------------
check : test/testik.o main.o
	gcc test/testik.o main.o -o check

test/testik.o : test/testik.c
	gcc -c test/testik.c -o test/testik.o

main.o : main.c
	gcc -c main.c -o main.o
# end of check makefile -------------------

$(BUILDDIR)/mem.o: $(SRCDIR)/mem.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/mem_debug.o: $(SRCDIR)/mem_debug.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/util.o: $(SRCDIR)/util.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/testik.o: $(TESTIKDIR)/testik.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/main.o: main.c build
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILDDIR)

