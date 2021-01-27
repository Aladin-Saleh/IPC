CC = gcc -Wall 

all : main incr

incr : incr.c 
	$(CC) incr.c -o incr

main : main.c 
	$(CC) main.c -o main

clean :
	rm -f incr main
