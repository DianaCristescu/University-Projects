build:
	gcc -std=c9x test.c -lm -Wall -D_GNU_SOURCE -o test

debug:
	gcc -g -std=c9x test.c -lm -Wall -D_GNU_SOURCE -o test_debug

test:
	./test

clean:
	rm -f test test_debug

.PHONY: test