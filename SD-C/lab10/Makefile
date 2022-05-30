.phony: build run test clean

CC = gcc
CFLAGS= -Wall -std=c99
CFLAGS_DEBUG= -Wall -std=c99 -g
DEPS = $(wildcard *.h)
OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
EXEC = testGraph
EXEC_DEBUG= testGraph.out

build: $(EXEC)
		
$(EXEC):$(OBJ)
	gcc $(CFLAGS) -o $@ $^

debug: $(EXEC_DEBUG)
		
$(EXEC_DEBUG):$(OBJ)
	gcc $(CFLAGS_DEBUG) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c  $< -o $@ 

run:
	./$(EXEC)

test:
	valgrind --leak-check=full ./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJ) $(EXEC_DEBUG)
