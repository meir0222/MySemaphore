# SemaphoreProj
This program demonstrates process synchronization using System V semaphores in C.
The program creates five child processes, each executing sequentially without overlapping, ensuring controlled execution using semaphores.

How It Works
* A set of 5 semaphores is created using semget().
* Each semaphore is initialized with decreasing values (4, 3, 2, 1, 0).
* Child processes are created, each modifying a shared resource while ensuring synchronization.
* Semaphore operations (semop) allow processes to wait (P operation) and signal (V operation), preventing race conditions.
* Once all child processes finish execution, the semaphores are cleaned up using semctl(IPC_RMID).
