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
    char * endPtr;

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
    *count = strtol(argv[1],&endPtr,10 )+ (*count)++;
    printf("recu: %d\n----------\n",*count);




    return 0;
}