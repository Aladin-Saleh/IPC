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
	char buf[256];
	int smid,shmid;
	
	int *count;


	if ((cle = ftok("/tmp",1)) == -1)
	{
		erreur("Erreur lors de la creation de la clé...");
	}

	if ((smid = semget(cle,1,0666|IPC_CREAT)) == -1)
	{
		erreur("Erreur lors de la creation de la semaphore...");
	}
	
	shmid=shmget(cle, sizeof(int), IPC_CREAT|0666);
	assert(shmid >= 0);

	count = (int*)shmat(shmid,NULL,0);
	assert(count != (void*)-1);

	*count = 0;

	u.val = 1;

	if(semctl(smid, 0, SETVAL, u) < 0)
    {
        erreur("Erreur semclt....");
    }

 
		for(int i = 1; i < 6; ++i)
        {
            
    		pid_t pid = fork();
			assert( pid != -1);

			if (pid==0) {
			snprintf(buf,sizeof(buf),"%d",i);
			execl("./incr","./incr",buf,NULL);
			assert(0);//Pas censé arriver ici
			}

			

            sleep(rand() % 2);
        }
	
	

	for (int i=1;i<=6;i++) wait(NULL);

	printf("count = %d\n",*count);
	assert(shmctl(shmid,IPC_RMID,0) >=0);
	return 0;
}


void erreur(char* texte)
{
	printf("%s\n",texte );
	exit(-1);
}