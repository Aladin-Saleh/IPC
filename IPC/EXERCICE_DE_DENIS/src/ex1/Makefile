CC = gcc -Wall 

all : main incr main_semaphore

main_semaphore : main_semaphore.c
	$(CC) main_semaphore.c -o main_semaphore

incr : incr.c 
	$(CC) incr.c -o incr

main : main.c 
	$(CC) main.c -o main

clean :
	rm -f incr main main_semaphore
