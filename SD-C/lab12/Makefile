.phony: build clean

build: words

words: words.c
	gcc -g -o words words.c

run:
	./words 32 fileA fileB

clean:
	rm -f words
