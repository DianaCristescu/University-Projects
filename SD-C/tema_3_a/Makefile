build:
	gcc -Wall bayesien.c -o bnet

debug:
	gcc -g -Wall bayesien.c -o bayesien.out

run:
	valgrind --leak-check=full ./bnet teste/cerinta2/test0.in rez.out

test:
	./_checker.sh

clean:
	rm bayesien.out rez.out bnet.in bnet rezultat_valgrind.txt bnet.out