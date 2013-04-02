#CC=gcc
CC=arm-linux-gcc
6410ForInternet:board.o
	$(CC) -o 6410ForInternet	board.o -lpthread
board.o:board.c
	$(CC) -c board.c 
clean:
	rm -rf *.o
