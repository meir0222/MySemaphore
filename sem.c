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

union semun {
    int val;
    struct semid_ds *buff;
    unsigned short *array;
};
union semun semarg;

int main(void) {
    int i, j, k, status;

    // Create semaphore set with 5 semaphores
    semid = semget(IPC_PRIVATE, 5, 0600);
    if (semid == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphores
    for (i = 0; i < 5; i++) {
        semarg.val = 4 - i;
        if (semctl(semid, i, SETVAL, semarg) == -1) {
            perror("semctl failed");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < 5; i++) {
        if (fork() == 0) {
            struct sembuf sop = { i, -4, 0 }; // Decrease semaphore by 4
            for (j = i + 1; j < 101; j += 5) {
                if (semop(semid, &sop, 1) == -1) {
                    perror("semop failed");
                    exit(EXIT_FAILURE);
                }
                printf("Semaphore %d: %d\n", i, j);

                // Release other semaphores
                for (k = 0; k < 5; k++) {
                    if (k != i) {
                        sops[0].sem_op = 1;
                        sops[0].sem_num = k;
                        if (semop(semid, sops, 1) == -1) {
                            perror("semop release failed");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }
            exit(0);
        }
    }

    // Wait for all child processes
    while (wait(&status) > 0);

    // Remove semaphore set
    if (semctl(semid, 0, IPC_RMID, semarg) == -1) {
        perror("semctl remove failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
