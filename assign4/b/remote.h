/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _REMOTE_H_RPCGEN
#define _REMOTE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define REMOTE_PROG 0x31423456
#define REMOTE_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define STR_REMOTE 1
extern  char ** str_remote_1(long *, CLIENT *);
extern  char ** str_remote_1_svc(long *, struct svc_req *);
extern int remote_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define STR_REMOTE 1
extern  char ** str_remote_1();
extern  char ** str_remote_1_svc();
extern int remote_prog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_REMOTE_H_RPCGEN */
