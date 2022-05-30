make:
	javac ./src/Bucket.java ./src/Graph.java ./src/ShortestPath.java ./src/Weight.java

run-p1:
	tests=$$(ls ./in | grep -E "test[0-9]+.in");\
	for i in $$tests; do\
		java -cp ./src ShortestPath 1 ./in/"$$i";\
	done

run-p2:
	tests=$$(ls ./in | grep -E "test[0-9]+.in");\
	for i in $$tests; do\
		java -cp ./src ShortestPath 2 ./in/"$$i";\
	done

run-p3:
	tests=$$(ls ./in | grep -E "test[0-9]+.in");\
	for i in $$tests; do\
		java -cp ./src ShortestPath 3 ./in/"$$i";\
	done

run-best:
	tests=$$(ls ./in | grep -E "test[0-9]+.in");\
	for i in $$tests; do\
		java -cp ./src ShortestPath 1 ./in/"$$i";\
	done

clean:
	rm ./src/*.class
	rm ./out/*.out