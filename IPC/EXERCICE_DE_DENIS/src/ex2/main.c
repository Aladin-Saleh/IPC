#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<string.h>
#include<assert.h>
#include <sys/types.h>
#include <sys/wait.h>


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

struct sembuf p = { 0, -1, SEM_UNDO};
struct sembuf v = { 0, +1, SEM_UNDO};

void erreur(char* texte);


void run(int no,int nbproc,int semid,char *m){
	int len = strlen(m);
	int i,j;
	for(j=1;j<=10;j++){
		for (i=0;i<len;i++){
			usleep(10000);
			write(1,m+i,1);
		}
	}
	exit(0);
}

int main(){

	char * s []={"first","second","third","fourth"};
	int semid,i;
	key_t cle;


	if ((cle = ftok("/tmp",1)) == -1)
	{
		erreur("Erreur lors de la creation de la clé...");
	}

	if ((smid = semget(cle,1,0666|IPC_CREAT)) == -1)
	{
		erreur("Erreur lors de la creation de la semaphore...");
	}

	u.val = 1;
	
	for(i=0;i<4;i++){
		if (fork() == 0) {
			run(i,4,semid,s[i]);
		}
	}

	for (int i=1;i<=4;i++) wait(NULL);


	printf("\n");

	//semctl(semid,0,IPC_RMID);
}




void erreur(char* texte)
{
	printf("%s\n",texte );
	exit(-1);
}