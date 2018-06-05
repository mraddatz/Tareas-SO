CC=gcc
CFLAGS=-c -Wall -I.
LDFLAGS= -lm
SOURCES=main.c poker.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=t4

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(patsubst %,obj/%,$^) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o obj/$@