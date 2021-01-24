/*producer_sm.c*/
/*
Ce programme lit une suite de nombre, et effectue le cumul dans une variable en mémoire partagée.*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>


struct data
{
    int nombre;
    int total;
};

int main(int argc, char const *argv[])
{
    key_t cle;    
    int id; 
    struct data* s_data;
    int reponse;

/* key_t ftok (const char *__pathname, int __proj_id)
    Generates key for System V style IPC.  */
/* char *getenv (const char *__name)
    Return the value of envariable NAME, or NULL if it doesn't exist.  */
    cle = ftok(getenv("HOME"),'A');

    if (cle == -1)
    {
        perror("Error ftok");
        exit(-1);
    }

    /*int shmget(key_t clé,size_t size,int shmflg)
    shmget() renvoie l'identifiant du segment de mémoire partagée associé à la valeur de l'argument clé. 
    Un nouveau segment mémoire, de taille size arrondie au multiple supérieur de PAGE_SIZE, est créé si clé a la valeur IPC_PRIVATE 
    ou si aucun segment de mémoire partagée n'est associé à clé, et IPC_CREAT est présent dans shmflg. */

    /*
    -IPC_CREAT
    pour créer un nouveau segment. Sinon shmget() recherchera le segment associé à clé, vérifiera que l'appelant a la permission d'y accéder. 
    
    -IPC_EXCL est utilisé avec IPC_CREAT pour garantir l'échec si le segment existe déjà. 

    -mode d'accès
    (les 9 bits de poids faibles) indiquant les permissions pour le propriétaire, le groupe et les autres. 
    Ces bits ont le même format et la même signification que l'argument mode de open(2). 
    */    
    id = shmget(cle,sizeof(struct data),IPC_CREAT|IPC_EXCL|0666);
    if (id == -1)
    {
        perror("Error id = -1");
        exit(-1);
    }

    /*
    void *shmat (int __shmid, const void *__shmaddr, int __shmflg)
    Attach shared memory segment.
    */
    s_data = (struct data*)shmat(id,NULL,SHM_R|SHM_W);
    if (s_data == NULL)
    {
        perror("Error lors de la jointure des segments de mémoire");
        exit(-1);
    }

    /*Initialisation des données de la structures*/
    s_data->nombre = 0;
    s_data->total = 0;

    while (1)
    {
        printf("+");
        if (scanf("%d",&reponse) != 1)
        {
            break;
        }
        s_data->nombre++;
        s_data->total += reponse;
        printf("sous-total = %d = %d \n",s_data->nombre,s_data->total);

        

    }
    printf("---\n");
    
    /*On enleve la jointure de mémoire*/
    /*int shmdt (const void *__shmaddr)*/
    if (shmdt((char*)s_data) == -1)
    {
        perror("Error shmdt");
        exit(-1);
    }
    /*On supprime le segment de mémoire*/
    /*shmctl permet de gerer les action sur la memoire partagé*/
    /*int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf)*/
    if (shmctl(id,IPC_RMID,NULL) == -1)
    {
        perror("Error shmclt : lors de la suppression du segment mémoire");
        exit(-1);
        
    }

    return 0;
}

