all: Onegin

Onegin: Onegin_lib.o main.o
	gcc Onegin_lib.o main.o -o Onegin

Onegin_lib.o: Onegin_lib.c
	gcc Onegin_lib.c -c

main.o: main.c
	gcc main.c -c

clean:
	rm Onegin_lib.o main.o
