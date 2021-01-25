/*
 * Client : il charge une file de messages, envoie une requete
 * au serveur, attend la reponse, l'ecrit sur stdout
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <assert.h>
#include "types.h"

void usage(char *s){
    fprintf(stderr,"Usage : %s gauche op droite\n\tgauche, droite : entiers\n\top = +, -, *, /, \%% \n",s);
    exit(-1);
}

int main (int argc, char *argv[]){
    key_t cle;			/* cle de la file     */
    int file_mess;		/* ID de la file    */
    requete_t requete;
    reponse_t reponse;
	//ssize_t res_rcv;
    int gauche, droite; 
    char op;

    pid_t pid = getpid();


    /* Recuperation des arguments */
    if (argc < 4)
	usage(argv[0]);

    gauche = (int)strtol(argv[1],NULL,0);
    droite = (int)strtol(argv[3],NULL,0);

	op = argv[2][0];

	if (
			(op != '+') 
			&& (op != '-') 
			&& (op != '*') 
			&& (op != '/') 
			&& (op != '%')
	   )
		usage(argv[0]);

	/* Calcul de la cle	                 */
	cle = ftok(FICHIER_CLE,'a');
    //cle = ftok(getenv("HOME"),'A');
	
	assert(cle != -1);
	
	/* Recuperation file de message :    */
	file_mess = msgget(cle,0);
	assert(file_mess != -1);
	

	couleur(BLEU);
	fprintf(stdout, "\t\tLe client %d envoie : %d %c %d\n", pid, gauche, op, droite);
	couleur(REINIT);

	/* creation de la requete :          */
	requete.type = 1;
	requete.op = op;
	requete.d = droite;
	requete.g = gauche;
	requete.expediteur = getpid();

	/* envoi de la requete :             */

	msgsnd(file_mess,(const void *)&requete,sizeof(requete_t),0);// == -1)
	//{
	//	perror("Erreur lors de l'envoi de la requete !");
	//	exit(-1);
	//}
	

	/* attente de la reponse :           */
	msgrcv(file_mess,&reponse,sizeof(reponse_t)-sizeof(long),1,0);
	/*
	if (msgrcv(file_mess,&reponse,sizeof(reponse_t)-sizeof(long),1,0) == -1)
	{
		perror("Erreur lors de la recuperation de la reponse !");
		exit(-1);
	}
	
*/
	/* affichage de la reponse           */

	printf("Resultat : %d\n",reponse.resu);

	exit(0);
}



