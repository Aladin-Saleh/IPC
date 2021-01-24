/*consumer_sm.c*/
/*Ce programme affiche périodiquement le contenue de la mémoire partagée.
Il est arreté via SIG-INT*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


#define DELAI 3


struct data
{
    int nombre;
    int total;
};

int continuer = 1;

void arreter(int signal)
{
    continuer = 0;
    printf("Signal d'arret recu ! \n");
}

/*Ce programme va lire ce qui est écrit dans le segment de mémoire partagé ;
par le programme consommateur; avec la fonction ftock() on va récupérer la clé "cle", 
s'en servir pour recupérer son "id" à travers la fonction shmget(), */
int main(int argc, char const *argv[])
{
    key_t cle;
    int id;
    struct data* s_data;
    struct sigaction a;

    cle = ftok(getenv("HOME"),'A');

    if (cle == -1)
    {
        perror("Error ftok");
        exit(-1);
    }
    id = shmget(cle,sizeof(struct data),0);
    if (id == -1)
    {
        perror("Error id = -1");
        exit(-1);
    }

    s_data = (struct data*)shmat(id,NULL,SHM_R);
    if(s_data == NULL)
    {
        perror("error shmat");
        exit(-1);
    }

    a.sa_handler = arreter;
    sigemptyset(&a.sa_mask);
    a.sa_flags = 0;
    sigaction(SIGINT,&a,NULL);
    while (continuer)
    {
        sleep(DELAI);
        printf("sous-total = %d = %d \n",s_data->nombre,s_data->total);

    }

    printf("---\n");

     if (shmdt((char*)s_data) == -1)
    {
        perror("Error shmdt");
        exit(-1);
    }    
    
    return 0;
}
