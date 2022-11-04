#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main()
{

    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int *X = (int *)shmat(shmid, (void *)0, 0);
    *X = 0;
    printf("X: %d\n", *X);
    shmdt(X);

    int n1 = fork();

    if (n1 == 0)
    {
        printf("Child 1\n");

        for (int i = 0; i < 10; i++)
        {
            //sleep(1);
            int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
            int *X = (int *)shmat(shmid, (void *)0, 0);
            *X = *X + 1;
            printf("Value of X(child1)): %d\n", *X);
            shmdt(X);
        }
    }

    else
    {
        int n2 = fork();
        if (n2 == 0)
        {
            sleep(0.5);
            printf("Child 2\n");
            for (int i = 0; i < 10; i++)
            {
                //sleep(2);
                int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
                int *X = (int *)shmat(shmid, (void *)0, 0);
                *X = *X - 1;
                printf("Value of X(child2): %d\n", *X);

                shmdt(X);
            }
        }

        else
        {
            sleep(1);
            printf("Parent\n");
            int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
            int *X = (int *)shmat(shmid, (void *)0, 0);
            printf("Value of X(parent): %d\n", *X);
            shmdt(X);
        }
    }

    return 0;
}
