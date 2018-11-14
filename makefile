all: shell.o
	gcc -o shell shell.o -lreadline
shell.o: shell.c
	gcc -c shell.c -lreadline

clean:
	rm *.o

