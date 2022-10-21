// CSB19057
// Prasanjit Dutta

#include <stdio.h>
#include <pthread.h>
void *thread(void *vargp);

int main()
{
    pthread_t tid;
    printf("Hello world !\n");
    // create a thread
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);
    // wait for the thread to exit
    pthread_exit((void *)NULL);
}

void *thread(void *varg)
{
    printf("Hello World from thread\n");
    pthread_exit((void *)NULL);
}