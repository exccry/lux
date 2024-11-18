SRCDIR = ../src
INCDIR = ../include

SOURCES = $(SRCDIR)/network.c $(SRCDIR)/config.c $(SRCDIR)/log.c
INCLUDES = -I$(INCDIR)

test: test.c $(SOURCES)
	gcc -o test test.c $(SOURCES) $(INCLUDES) -Wall && ./test
clean:
	rm ./test
