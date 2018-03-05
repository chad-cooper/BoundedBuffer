p3: main.o
	gcc -o p3 main.o -lpthread -lrt

main.o: main.c
	gcc -c main.c 