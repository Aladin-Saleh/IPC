#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<string.h>
#include<assert.h>
#include <sys/types.h>
#include <sys/wait.h>


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


	for(i=0;i<4;i++){
		if (fork() == 0) {
			run(i,4,semid,s[i]);
		}
	}

	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);

	//semctl(semid,0,IPC_RMID);
}
