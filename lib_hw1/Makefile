CC=gcc
CFLAGS=-W -Wall -g
SOURCES=bitmap.c\
	hash.c\
	list.c\
	testlib.c\

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=testlib

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf *.o $(EXECUTABLE)
