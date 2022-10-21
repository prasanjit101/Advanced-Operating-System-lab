// CSB19057
// Prasanjit Dutta

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    pid_t pid;
    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        break;
    case 0:
        sleep(3);
        printf("I am the child process with ppid %d", getppid());
        break;
    default:
        printf("I am the parent process with pid %d\n", getpid());
        break;
    }
    return 0;
}
/* 
I am the parent process with pid 2188
I am the child process with ppid 11
 */