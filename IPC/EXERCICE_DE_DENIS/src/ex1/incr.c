#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>

#include <sys/wait.h>



int main(int argc, char const *argv[])
{
    key_t cle;
    int id;
    int *count;

    cle = ftok("/tmp",1);
    assert(cle != -1);

    id = shmget(cle,sizeof(int),0);
    if (id == -1)
    {
        perror("Error id = -1...");
        exit(-1);
    }

    count = (int*)shmat(id,NULL,0);
    assert(count != (void*)-1);
    
    int pas = strtol(argv[1],0,10);
    for (int i = 0; i < 10000; ++i)
    {
        (*count) += pas;
    }

    printf("recu: %d\n----------\n",*count);


    /*
    *Ici, comme tout les processus on acces à la variable count, il y'a des erreurs d'incrementation
    *Il faut utiliser des sémaphores pour rendre atomique l'utilisation de la variable partagées count 
    *On est supposé avoir un count egale à 150 000 si tout c'est bien passé
    */



    return 0;
}