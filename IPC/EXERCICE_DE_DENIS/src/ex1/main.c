#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
struct data
{
    int count;
    char buf[256];

};

int main(int argc,char * argv[])
{
	int shmid,
	    i;
	    //*count;
	struct data* s_data;

	//char buf[256];
	key_t k = ftok("/tmp",1);
	assert(k!=-1);

	shmid=shmget(k, sizeof(struct data), IPC_CREAT|0666);
	assert(shmid >= 0);

	s_data = (struct data*)shmat(shmid,NULL,0);
	assert(s_data!= /*(void*)*/NULL);

	s_data->count = 0;

	for (i=1;i<=5;i++){
		pid_t p = fork();
		assert( p != -1);

		if (p==0) {
           
			snprintf(s_data->buf,sizeof(s_data->buf),"%d",i);
            execl("./incr","./incr",s_data->buf,NULL);
            //printf("tu fous quoi ici ! \n");
			exit(-1);
            //assert(0);//n'est pas censé arrivé jusque la !
		}
	}

	for (i=1;i<=5;i++) wait(NULL);

	printf("count = %d\n",s_data->count);
	assert(shmctl(shmid,IPC_RMID,0) >=0);
}