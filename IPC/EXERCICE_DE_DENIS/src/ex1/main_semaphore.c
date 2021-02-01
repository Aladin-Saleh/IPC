#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>




//struct sem{
//ushort semval;  /* Valeur du sémaphore                  */
//ushort sempid;  /* PID dernière opération               */
//ushort semncnt; /* Nb proc. en attente incrément semval */
//ushort semzcnt; /* Nb proc. en attente de semval = 0    */
//}


/*
sem_flg est une combinaison d'indicateur qui peut contenir IPC_NOWAIT et SEM_UNDO . Ici nous supposons que sem_flg est 0.
sem_num indique le numéro du sémaphore individuel sur lequel porte l'opération.
sem_op est un entier destiné (sauf si il est nul) à être ajouté à la valeur courante semval du sémaphore. L'opération se bloque si «sem_op + semval < 0 ».*/

/*
int main(int argc,char * argv[])
{
	int smid,i,*count;
    union semun arg;
	char buf[256];
	

	
	


	key_t k = ftok("/tmp",1);
	assert(k!=-1);

	if ((smid = semget(k,1,0666|IPC_CREAT)) == -1)
	{
		perror("Erreur lors de la création de la sémaphore....");
		exit(-1);
	}
	

	arg.val = 1;


	if (semctl(smid, 0, SETVAL, arg) == -1) {
            perror("semctl");
            exit(1);
        }

}*/


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

struct sembuf p = { 0, -1, SEM_UNDO};
struct sembuf v = { 0, +1, SEM_UNDO};


void erreur(char* texte);


int main(int argc, char const *argv[])
{
	

	key_t cle;
	union semun u;
	int smid;
	int pid;


	if ((cle = ftok("/tmp",1)) == -1)
	{
		erreur("Erreur lors de la creation de la clé...");
	}

	if ((smid = semget(cle,1,0666|IPC_CREAT)) == -1)
	{
		erreur("Erreur lors de la creation de la semaphore...");
	}

	u.val = 1;

	if(semctl(smid, 0, SETVAL, u) < 0)
    {
        erreur("Erreur semclt....");
    }

    if ((pid = fork()) < 0)
    {
    	erreur("Erreur lors de la creation du fils...");
    }
    else
    {
    	char* s = "abcdefgh";
    	int length = strlen(s);

    	for (int i = 0; i < length; ++i)
    	{
    		if (semop(smid,&p,))
    		{
    			/* code */
    		}
    	}
    }










	return 0;
}



void erreur(char* texte)
{
	printf("%s\n",texte );
	exit(-1);
}