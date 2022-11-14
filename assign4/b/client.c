#include <stdio.h>
#include <rpc/rpc.h>
#include "remote.h"

int main()
{
    CLIENT *clnt;
    char **result;
    long argp = 1;

    clnt = clnt_create("localhost", REMOTE_PROG, REMOTE_VERS, "tcp");
    if (clnt == NULL)
    {
        clnt_pcreateerror("localhost");
        exit(1);
    }

    result = str_remote_1(&argp, clnt);
    if (result == (char **)NULL)
    {
        clnt_perror(clnt, "call failed");
    }
    printf("result: %s ", *result);
    exit(0);
}