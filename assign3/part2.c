#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SHM_SIZE 1024

int start = 0;
int main()
{
    int shmid, mode;
    char *data;
    key_t key = ftok("file.txt", 'R');
    int fd;
    pid_t child_a, child_b;
    child_a = fork();

    if (child_a == 0)
    {
        //A
        shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
        if (shmid == -1)
        {
            perror("error at shmid");
            exit(1);
        }

        for (int i = 1; i <= 10; i++)
        {
            data = shmat(shmid, (void *)0, 0);
            if (data == (char *)(-1))
            {
                perror("error at shmat");
                exit(1);
            }
            int val = atoi(data);
            printf("A: %d\n [iteration : %d]", val, i);
            sprintf(data, "%d", val);
            val++;
        }
        if (shmdt(data) == -1)
        {
            perror("shmdt");
            exit(1);
        }
    }
    else
    {
        // B
        child_b = fork();

        if (child_b == 0)
        {
            shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
            if (shmid == -1)
            {
                perror("shmid");
                exit(1);
            }
            for (int i = 1; i <= 10; i++)
            {
                data = shmat(shmid, (void *)0, 0);
                if (data == (char *)(-1))
                {
                    perror("shmat");
                    exit(1);
                }
                int val = atoi(data);
                printf("B: %d\n [iteration : %d]", val, i);
                val--;
                sprintf(data, "%d", val);
            }

            if (shmdt(data) == -1)
            {
                perror("shmdt");
                exit(1);
            }
        }
        else
        {
            // Parent
            sleep(2);
            shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
            if (shmid == -1)
            {
                perror("shmid");
                exit(1);
            }
            // synchronize the parent and child processes
            wait(NULL);
            wait(NULL);
            data = shmat(shmid, (void *)0, 0);
            if (data == (char *)(-1))
            {
                perror("shmat");
                exit(1);
            }
            printf("The value of the shared memory is %s \n", data);
            if (shmctl(shmid, IPC_RMID, NULL) == -1)
            {
                perror("shmctl");
                exit(1);
            }
        }
    }
    return 0;
}