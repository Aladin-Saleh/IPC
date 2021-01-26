/*
 * Serveur : il cr�e une file de messages, attend des messages 
 * des clients, y repond.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <assert.h>
#include "types.h"
#include <sys/stat.h>   /* Pour S_IRUSR, S_IWUSR */



int file_mess; /* ID de la file, necessairement global
				  pour pouvoir la supprimer a la terminaison */
key_t cle; /* cle de la file     */

//int continuer_boucle = 1;

void arret(int s){
  /* Suppression de la file */
	

  if(msgctl(file_mess, IPC_RMID, NULL) == -1) {
    perror("Erreur lors de la suppression de la file ");
    exit(EXIT_FAILURE);
  }
 	printf("File supprimee.\n");
 
  	exit(EXIT_SUCCESS);

}

int set_signal_handler(int signo, void (*handler)(int)) {
	struct sigaction sa;
	sa.sa_handler = handler;    // call `handler` on signal
	sigemptyset(&sa.sa_mask);   // don't block other signals in handler
	sa.sa_flags = 0 ;            //  don't restart system calls
	return sigaction(signo, &sa, NULL);
}

int effectuer(char op, int g, int d){
	switch (op){
		case '+':
			return g+d;

		case '-':
			return g-d;
		case '*':
			return g*d;

		case '/':
			return g/d;

		default :
			return g%d;
	}
}
int main (int argc, char *argv[]){
	requete_t requete;
	reponse_t reponse;
//	ssize_t nb_lus;

	/* cacul de la cle de la file    */
	cle = ftok(FICHIER_CLE,'a');


	assert(cle != -1);

	/* Creation file de message :    */

	file_mess = msgget(cle,0666|IPC_CREAT);
	requete.type = 1;
	printf("file_mess = %d \n",file_mess);
	//sleep(10);
	
	assert( file_mess != -1);

	assert(set_signal_handler(SIGINT,arret) == 0);

	srand(getpid());


	while(1){ 

		/* serveur attend des requetes, de type 1 :        */
		printf("Attente...\n");
	 	
		if (msgrcv(file_mess,&requete, sizeof(requete_t) - sizeof(long), 1,0)== -1 )
		{
			perror("Erreur lors de la reception...");
			exit(-1);
		}
		
		printf("Serveur : Reception en attente...\n");
		/* traitement de la requete :                      */
		reponse.type = requete.expediteur;
		reponse.resu = effectuer(requete.op,requete.g,requete.d);

		/* Attente aleatoire */

		sleep(5);

		/* envoi de la reponse :                           */
		msgsnd(file_mess,&reponse, sizeof(reponse_t) - sizeof(long),IPC_NOWAIT);
		printf("Reponse du serveur envoyé ! \n");
	}



	

	return EXIT_SUCCESS;
}


