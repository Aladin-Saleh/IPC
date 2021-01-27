# Mécanismes IPC. Mémoire partagée, sémaphores, synchronisation

#### Ex1. 
**Atomicité**. Le but de l'exercice de mettre en évidence que l'incrémentation d'une variable  n'est pas en générale atomique. 
Le processus `main` crée un compteur partagé, et lance 5 exemplaires d'un processus qui incrémente 100000 fois la valeur du compteur 
respectivement de  i=1,2,3,4 et 5.
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char * argv[])
{
	int shmid,
	    i,
	    *count;
	
	char buf[256];
	key_t k = ftok("/tmp",1);
	assert(k!=-1);

	shmid=shmget(k, sizeof(int), IPC_CREAT|0666);
	assert(shmid >= 0);

	count = (int*)shmat(shmid,NULL,0);
	assert(count != (void*)-1);

	*count = 0;

	for (i=1;i<=5;i++){
		pid_t p = fork();
		assert( p != -1);

		if (p==0) {
			snprintf(buf,sizeof(buf),"%d",i);
			execl("./incr","./incr",buf,NULL);
			assert(0);
		}
	}

	for (i=1;i<=5;i++) wait(NULL);

	printf("count = %d\n",*count);
	assert(shmctl(shmid,IPC_RMID,0) >=0);
}
```

1. Écrire le code `incr.c` du processus `incr`.
2. Quelle est la valeur finale du compteur ? Quelle serait-elle si l'incrémentation
   de la variable partagée était atomique ?
3. Écrire une version qui utilisent un sémaphore binaire pour assurer l'exclusion mutuelle lors de l'accès à la variable partagée. 


#### Ex2.
**Sequentialisation**. Le programme suivant crée 4 fils, qui affiche en boucle sur leur sortie standard, caractère par caractère, un message quelconque.

```c
void run(int no,int nbproc,int semid,char *m)
{
	int len = strlen(m);
	int i;
	while(1){
		for (i=0;i<len;i++){
			usleep(rand()%10000);
			write(1,m+i,1);
		}
	}
}
int main()
{
	char * s []={"first","second","third","fourth"};
	int semid,i;


	for(i=0;i<4;i++){
		if (fork() == 0) {
			run(i,4,semid,s[i]);
		}
	}

	pause();
	//semctl(semid,0,IPC_RMID);
}
```

1. A l'aide d'un sémaphore binaire, faites en sorte qu'il n'y ait pas d'entrelacement dans l'affichage des messages.
2. A l'aide d'un ensemble de sémaphores (combien ?), sequentialisez l'affichage des messages dans l'ordre
   first, second,third, fourth,first, second, etc.

#### Ex3.

On dispose d'un compteur entier partagé entre plusieurs processus :

- des processus `Incr` qui peuvent incrémenter le compteur de 1.
- un processus `Reset` qui surveille la valeur du compteur. Il remet la valeur du compteur à 0 dès qu'elle atteint 
  une valeur VMAX que lui seul connaît.

Vous devez organiser (à l'aide de sémaphores) le fonctionnement du système de processus pour 3 exemplaires de `Incr`, et un `Reset`, quelque
  soit leur rythme de travail :

 Une trace de l'exécution avec `VMAX = 5`

```
---- this is incr1 -----
---- this is incr2 -----
---- this is incr3 -----
==== this is reset ====
incr1 : before x=0
        after  x=1
incr2 : before x=1
        after  x=2
incr3 : before x=2
        after  x=3
incr1 : before x=3
        after  x=4
incr2 : before x=4
        after  x=5
reset : before x=5
        after  x=0
incr3 : before x=0
        after  x=1
incr1 : before x=1
        after  x=2
incr2 : before x=2
        after  x=3
incr3 : before x=3
        after  x=4
incr1 : before x=4
        after  x=5
reset : before x=5
        after  x=0
incr2 : before x=0
        after  x=1
incr3 : before x=1
        after  x=2
incr1 : before x=2
        after  x=3
incr2 : before x=3
        after  x=4
incr3 : before x=4
```

