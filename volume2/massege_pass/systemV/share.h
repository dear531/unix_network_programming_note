#ifndef __SHARE_H__
#define __SHARE_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG_KEY		1234L
#define C_TO_S_TYPE	1

#if 0
	int msgget(key_t key, int msgflg);
#endif
int msgget_func(key_t key, int msgflg)
{
	int msgid;
	if (0 > (msgid = msgget(key, msgflg))) {
		fprintf(stderr, "msgget error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return msgid;
}
ssize_t msgrcv_func(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
{
	ssize_t rcvn;
	if (0 > (rcvn = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg))) {
		fprintf(stderr, "msgrcv error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return rcvn;
}
#if 0
	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
#endif
int msgsnd_func(int msqid, const void *msgp, size_t msgsz, int msgflg)
{
	int ret;
	if (0 > (ret = msgsnd(msqid, msgp, msgsz, msgflg))) {
		fprintf(stderr, "msgsnd error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
#endif
