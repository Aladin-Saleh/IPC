CC = gcc -Wall 

all : serveur client

client : client.c types.h
	$(CC) client.c -o client

serveur : serveur.c types.h
	$(CC) serveur.c -o serveur
	touch cle.serv

clean :
	rm -f serveur client cle.serv
