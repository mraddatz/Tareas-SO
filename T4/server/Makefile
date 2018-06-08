CC=gcc
CFLAGS=-c -Wall -I.
LDFLAGS= -lm
SOURCES=server.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=server

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(patsubst %,obj/%,$^) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o obj/$@