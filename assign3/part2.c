#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;

int main()
{
    // using semaphores.
    int shmid;
    int *shm;
    int *s;
    int i;
    int pid;
    int status;

    // create a shared memory segment
    shmid = shmget(IPC_PRIVATE, 1 * sizeof(int), 0777 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }
    // attach the shared memory segment
    shm = shmat(shmid, NULL, 0);
    if (shm == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }
    // initialise the semaphore
    sem_init(&mutex, 0, 1);
    // create a child process
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // child process
        // for loop accessing the shared variable
        sem_wait(&mutex);
        printf("child process -\n");
        for (i = 0; i < 10; i++)
        {
            *shm = *shm + 1;
            printf("[child] value for iteration %d - %d\n", i, *shm);
        }
        sem_post(&mutex);
        exit(0);
    }
    else
    {
        // parent process
        sleep(1);
        printf("parent process -\n");
        // for loop accessing the shared variable decrementing the shared variable
        sem_wait(&mutex);
        for (i = 0; i < 10; i++)
        {
            *shm = *shm - 1;
            printf("[parent] value for iteration %d - %d\n", i, *shm);
        }
        sem_post(&mutex);
        // wait for the child process to finish
        wait(&status);
    }
    // detach the shared memory segment
    shmdt(shm);
    // remove the shared memory segment
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
