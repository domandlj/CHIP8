CC = gcc
FLAGS = -g -Wall

default: chip8

screen.o:
	$(CC) $(FLAGS) -c screen.c
 
chip8.o: screen.o
	$(CC) $(FLAGS) -c chip8.c

chip8: main.c chip8.o screen.o
	$(CC) $(FLAGS) -o chip8 main.c screen.o chip8.o -lSDL2

clean:
	-rm *.o chip8 
	-rm -r *.dSYM
