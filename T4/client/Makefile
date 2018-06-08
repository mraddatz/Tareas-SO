CC=gcc
CFLAGS=-c -Wall -I.
LDFLAGS= -lm
SOURCES=client.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=client

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(patsubst %,obj/%,$^) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o obj/$@