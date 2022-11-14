#include <stdio.h>
#include <rpc/rpc.h>
#include "remote.h"
#include <time.h>
// return the current time as string
char **str_remote_1_svc(long *argp, struct svc_req *rqstp)
{
    static char *result;
    time_t t;
    time(&t);
    result = ctime(&t);
    return &result;
}