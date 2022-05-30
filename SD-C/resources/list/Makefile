build: list.c
	gcc -Wall -o list list.c

run: list
	vallgrind --leak-check=full ./list

clean:
	rm ./list