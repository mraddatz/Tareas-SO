CC=gcc
CFLAGS=-c -Wall -I.
LDFLAGS= -lm
SOURCES=p2.c structs.c optimize.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=simulator

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(patsubst %,obj/%,$^) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o obj/$@