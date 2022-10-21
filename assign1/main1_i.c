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
        printf("I am the child process with pid %d", getpid());
        printf("\nChild process terminated\n");
        exit(0);
        break;
    default:
        printf("I am the parent process with pid %d\n", getpid());
        sleep(3);
        system("ps ajx |grep main1");
        break;
    }
    return 0;
}
/* 
I am the parent process with pid 1895
I am the child process with pid 1896
Child process terminated
 1731  1895  1895  1731 pts/4     1895 S+    1000   0:00 /home/jit/academics/AOSlab/assign1/main1
 1895  1896  1895  1731 pts/4     1895 Z+    1000   0:00 [main1] <defunct>
 1895  1927  1895  1731 pts/4     1895 S+    1000   0:00 sh -c ps ajx |grep main1
 1927  1929  1895  1731 pts/4     1895 S+    1000   0:00 grep main1
  */