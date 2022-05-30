CC = gcc
CFLAGS = -Wall
EXE = sudoku
OBJ = src/sudoku.c

build: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) 

run:
	read -p "input file: " INPUT ; ./$(EXE) $${INPUT} 123 manual_mode

run1:
	./$(EXE) ./checker/input/task1/board01.json 123 manual_mode

hex1.1:
	read -p "tail size: " SIZE1; hexdump -C ./output_task1_board01.bmp | tail -$${SIZE1}

hex2.1:
	read -p "tail size: " SIZE2 ; hexdump -C ./output_task2_board01.bmp | tail -$${SIZE2}

clean:
	rm -f output*
	rm -f $(EXE)
	rm -f ./checker/out/task?/out*