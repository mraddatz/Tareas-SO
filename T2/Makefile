CC=gcc
CFLAGS=-c -Wall -I.
LDFLAGS= 
SOURCES=p2.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=t2p2

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(patsubst %,obj/%,$^) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o obj/$@