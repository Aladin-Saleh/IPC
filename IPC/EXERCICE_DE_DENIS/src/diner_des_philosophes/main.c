#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

int run_philo(int no,int nb_philo);
int set_signal_handler(int signo, void (*handler)(int));
void arret(int signum);
void erreur(char* message_erreur);

int id_sem;
key_t cle;


struct sembuf rendez_vous[] = {{nb_philo, -1, 0}, {nb_philo, 0, 0}};
struct sembuf fourchette[] = {{0, -1, 0}, {0, -1, 0}, {0, +1, 0}, {0, +1, 0}};




int main(int argc, char const *argv[])
{

	int nb_philo;
	int nb_assiette;
	int nb_baguette;
	int* val;
	pid_t p;

	if (argc > 2)
	{
		erreur("Usage : <nombre_de_philosophe>");
	}

	nb_philo = strtol(argv[1],0,10);
	nb_assiette = nb_baguette = nb_philo;


	/*
	*Le IPC_PRIVATE est la parce que il n'y a pas de fork ou d'exec, donc la sémaphores et la mémoires sont "herité"
	*Tout ce fait en récuperant l'id
	*/
	if ((id_sem = semget(IPC_PRIVATE, nb_philo + 1, 0666)) == -1)
	{
		erreur("Erreur lors de la création de la sémaphores");
	}
	printf("Create semaphores success...\n");


	val = (int*) calloc (nb_philo + 1, sizeof(int));
	for (int i = 0; i < nb_philo; ++i)
	{
		val[i] = 1;//semaphores des fourchettes
	}

	val[nb_philo] = nb_philo;
	semclt(id_sem,0,SETALL,val);


	for (int i = 0; i < nb_philo; ++i)
	{
		if ((p = fork()) == -1)
		{
			erreur("Erreur avec le fork...");
		}
		if (p == 0)
		{
			run_philo(i,nb_philo);
		}
	}

	for (int i = 0; i < nb_philo; ++i)
	{
		wait(NULL);
	}

	semclt(id_sem,IPC_RMID,0);
	
	return 0;
}





int set_signal_handler(int signo, void (*handler)(int)) {
	struct sigaction sa;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	return sigaction(signo, &sa, NULL);
}



void arret(int signum) {
	printf("Signal d'arret recu ! \n");
	exit(EXIT_SUCCESS);
}

void erreur(char* message_erreur)
{
	printf("%s\n",message_erreur );
	exit(-1);
}