build: games.c
	gcc -Wall -o games games.c
normal: games
	chmod 777 ./games
	./games
clean:
	rm ./games ./in ./out
run: games
	valgrind --leak-check=full ./games