CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g 
SOURCES = grafo.c coloreo.c queue.c main.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = main

.PHONY: clean

all: $(TARGET)

test: $(TARGET)
	./$(TARGET) -vf input/*.in

v: $(TARGET)
	valgrind --leak-check=full --show-reachable=yes ./$(TARGET) -vf input/*.in

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)


