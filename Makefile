CC=gcc
#CFLAGS=-Ofast -Wall #Use these parameters for optimized builds
CFLAGS=-O0 -g -Wall #Use these parameters for debugging
SOURCES=check_mm.c mm.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run_tests

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ -lcheck

clean:
	rm -rf *o *~ $(EXECUTABLE)
