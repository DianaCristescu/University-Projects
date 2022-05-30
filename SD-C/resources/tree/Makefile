build: tree.c
	gcc -Wall -o tree tree.c

run: tree
	vallgrind --leak-check=full ./tree

clean:
	rm ./tree