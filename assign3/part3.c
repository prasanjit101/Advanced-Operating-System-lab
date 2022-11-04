#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;

void *producer(void *arg)
{
    sem_wait(&mutex);
    printf("producer..\n");
    sleep(3);
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int *b = (int *)shmat(shmid, 0, 0);
    for (int i = 0; i < 50;)
    {
        sleep(2);
        for (int j = 0; j < 20 && i < 50; j++)
        {
            b[20] = b[20] + b[j];
            i++;
            printf(" %d ", b[j]);
        }
        printf("\n");
    }
    shmdt(b);
    sem_post(&mutex);
}

void *consumer(void *arg)
{
    sem_wait(&mutex);
    printf("consumer..\n");
    sleep(3);
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int *a = (int *)shmat(shmid, 0, 0);
    for (int i = 0; i < 50;)
    {
        sleep(2);
        for (int j = 0; j < 20 && i < 50; j++)
        {
            a[j] = (i + 1);
            i++;
        }
    }
    shmdt(a);
    sem_post(&mutex);
}

void *thread3(void *arg)
{
    sem_wait(&mutex);
    printf("thread3..\n");
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int *a = (int *)shmat(shmid, (void *)0, 0);
    printf("Value of SUM): %d\n", a[20]);

    sem_post(&mutex);
}

int main()
{
    key_t key = ftok("shmfile", 65);

    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int *a = (int *)shmat(shmid, (void *)0, 0);
    *a = 0;
    shmdt(a);
    int m, n;
    // pthread_t t[10];

    printf("m and n: \n");
    scanf("%d%d", &m, &n);
    sem_init(&mutex, 0, 1);
    pthread_t t[m + n];
    pthread_t t3;

    for (int i = 0; i < m; i++)
    {
        pthread_create(&(t[i]), NULL, producer, NULL);
        sleep(1);
    }

    for (int i = m; i < (m + n); i++)
    {
        pthread_create(&(t[i]), NULL, consumer, NULL);
        sleep(1);
    }

    pthread_create(&t3, NULL, thread3, NULL);

    for (int i = 0; i < m; i++)
    {
        pthread_join(t[i], NULL);
        sleep(1);
    }

    for (int i = m; i < (m + n); i++)
    {
        pthread_join(t[i], NULL);
        sleep(1);
    }

    pthread_join(t3, NULL);

    sem_destroy(&mutex);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
