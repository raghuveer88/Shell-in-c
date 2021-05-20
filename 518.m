all : 518.exe

518.exe : 518.o
	gcc -o 518.exe 518.o
518.o:518.c
	gcc -c 518.c
clean:
	rm 518.o 518.exe
