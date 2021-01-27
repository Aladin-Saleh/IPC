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

int main(int argc, char const *argv[])
{
    key_t cle;
    int id;
    struct data* s_data;
    //char *buf;


    cle = ftok("/tmp",1);
    assert(cle != -1);


    id = shmget(cle,sizeof(struct data),0);
    if (id == -1)
    {
        perror("Error id = -1...");
        exit(-1);
    }



    s_data = (struct data*)shmat(id,NULL,SHM_R);
    
    
    
      printf("Avant incrémentation : %s\n",s_data->buf);
      s_data->count++;
//    printf("Apres incrémentation : %d\n----------\n",*count);

    
        


  
  if (shmctl(id,IPC_RMID,NULL) == -1)
    {
        perror("Error shmclt : lors de la suppression du segment mémoire");
        exit(-1);
        
    }







    
    return 0;
}
