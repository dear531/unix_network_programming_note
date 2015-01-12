#include <stdio.h>

#include "share.h"

int main(int argc, char *argv[])
{
	int msgid;
	msgid = msgget_func(MSG_KEY, 0);
#if 0
	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
#endif
	struct msgbuf_ctos {
		long mtype;
		int pid;
	} msgbuf_ctos;
	struct msgbuf_stoc {
		long mtype;
		char mtext[20];
	} msgbuf_stoc;
	pid_t pid;
	for ( ; ; ) {
		msgbuf_ctos.mtype = C_TO_S_TYPE;
		msgbuf_ctos.pid = pid = getpid();
		msgsnd_func(msgid, &msgbuf_ctos, sizeof(msgbuf_ctos), 0);
		msgrcv_func(msgid, &msgbuf_stoc, sizeof(msgbuf_stoc), pid, 0);
		fprintf(stdout, "received from server data :%s\n",
				msgbuf_stoc.mtext);
		sleep(1);
	}
	return 0;
}
