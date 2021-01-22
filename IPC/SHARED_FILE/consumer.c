#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/*
Un consommateur doit obtenir au moins un verrou partagé sur le fichier avant de lire le fichier.
Plusieurs lecteurs peuvent détenir un verrou partagé en même temps, 
mais aucun auteur ne peut accéder à un fichier lorsque même un seul lecteur détient un verrou partagé
*/

#define FILENAME "ipc_file.txt"

int main(int argc, char const *argv[])
{
    struct flock lock;
    lock.l_type = F_WRLCK; /* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    lock.l_pid = getpid();/*/* Process holding the lock. (celui qui maintient le lock du fichier) */
    lock.l_start = 0;/* Offset where the lock begins.  */
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;/* Size of the locked area; zero means until EOF.  */

    int fd;/*File descriptor*/
    if ((fd = open(FILENAME,O_RDONLY)) < 0 )
    {
        perror("Erreur lors de l'ouverture du fichier");
        exit(-1);//FAILURE
    }


    
        /*
        int fcntl(int fd, int cmd);
        int fcntl(int fd, int cmd, long arg);
        int fcntl(int fd, int cmd, struct flock * lock);


        La fonction  fcntl  permet  de  se  livrer  à  diverses
        opérations sur le descripteur de fichier fd.   L'opération
        en question est déterminée par la valeur de l'argument cmd*/

        /*
        F_SETLK  active  le  verrou  (si  l_type  vaut  F_RDLCK ou F_WRLCK) 
        ou efface  le  verrou  (si  l_type  vaut F_UNLCK).   
        Si  le  verrou est tenu par quelqu'un d'autre, cet appel renvoie -1 et positionne errno aux valeurs EACCES ou EAGAIN.*/

    fcntl(fd,F_GETLK,&lock);
    if (lock.l_type != F_UNLCK)
    {
        perror("Le fichier est toujours en train d'etre rempli...");
        exit(-1);
    }
    
    lock.l_type = F_RDLCK;
     if (fcntl(fd,F_SETLK,&lock)  < 0)
    {
        perror("Erreur lors de la pose du verrou du File descriptor,il se peut qu'un verrou soit deja posé ");
        exit(-1);
    }

    int buffer;
    while(read(fd,&buffer,1) > 0)
    {
        write(STDOUT_FILENO,&buffer,1);
    }
    printf("\n");
    fprintf(stderr,"Le processus %d a été executé avec succès et a recuperer dans le fichier...\n",lock.l_pid);


    /*On enlève le verrou sur le fichier*/
    lock.l_type = F_UNLCK;

      if (fcntl(fd,F_SETLK,&lock)  < 0)
    
        perror("Erreur lors du release du verrou du File descriptor...");
        exit(-1);
    


    close(fd);
    return 0;
}





