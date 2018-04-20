CC=gcc
CFLAGS=-c -Wall -I.
LDFLAGS= 
SOURCES=mlfq.c linkedlist.c arraylist.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mqlf

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(patsubst %,obj/%,$^) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o obj/$@