// Meir Hamou - 316132190
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int semid;
struct sembuf sops[1];

union semun
{
    int val;
    struct semid_ds * buff;
    unsigned short *array;
};
union semun semarg;

void main()
{
    int i, j, k, status, num = 1;

    semid=semget(IPC_PRIVATE, 5, 0600);

    for(i = 0; i < 5; i++)
    {
        semarg.val = 4 - i;
        semctl(semid, i, SETVAL, semarg);
    }

    sops->sem_num = 0;
    sops->sem_flg = 0;

    for (i = 0; i < 5; i++)
    {
        if (fork() == 0)
        {
            struct sembuf sop = { i, -4, 0};
            for (j = i + 1; j < 101; j += 5)
            {
                semop(semid, &sop, 1);
                printf("semaphore %d: %d\n",i,j);
                for(k = 0; k < 5; k++)
                {
                    if(k != i) // checking that processes are not overlapping other and running accordingly on time.
                    {
                        sops->sem_op = 1;
                        sops->sem_num = k;
                        semop ( semid , sops , 1 );
                    } 
                }
            }
            exit(0);    // child is done
        }
    }
    do ; while (wait(&status) > 0); // wait till all children are finished
    semctl ( semid , 0 , IPC_RMID , semarg );
}
