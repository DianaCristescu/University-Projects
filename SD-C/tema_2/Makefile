make debug:
	gcc -g hierarchy.c tema2.c -o tema2.out

build:
	gcc hierarchy.c tema2.c -o tema2

test: build
	valgrind --leak-check=full ./tema2

clean:
	rm *.out tema2