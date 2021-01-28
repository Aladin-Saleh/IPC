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
	int shmid,i,*count;
	char buf[256];


	key_t k = ftok("/tmp",1);
	assert(k!=-1);

	shmid=shmget(k, sizeof(int), IPC_CREAT|0666);
	assert(shmid >= 0);

	count = (int*)shmat(shmid,NULL,0);
	assert(count != (void*)-1);

	*count = 0;

	for (i=1;i<=10;i++){
		pid_t p = fork();
		assert( p != -1);

		if (p==0) {
			snprintf(buf,sizeof(buf),"%d",i);
			execl("./incr","./incr",buf,NULL);
			assert(0);//Pas censé arriver ici
		}
	}

	for (i=1;i<=10;i++) wait(NULL);

	printf("count = %d\n",*count);
	assert(shmctl(shmid,IPC_RMID,0) >=0);
}