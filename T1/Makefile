CC=gcc
CFLAGS=-c -Wall -I.
LDFLAGS= 
SOURCES=mlfq.c deque.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=t1

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(patsubst %,obj/%,$^) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o obj/$@