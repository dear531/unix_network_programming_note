#include <stdio.h>

#include "share.h"
#define FORK_C_TO_S	2L
#define FORK_S_TO_C 3L

int main(int argc, char *argv[])
{
	/* create msgid for share */
	int msgid;
	msgid = msgget_func(MSG_KEY, 0);
	/* create msgid for private for client */
	int fork_msgid;
	fork_msgid = msgget_func(IPC_PRIVATE, IPC_CREAT);
	/* send private msgid to server */
	struct msgbuf_ctos_id {
		long mtype;
		int msgid;
	} msgbuf_ctos_id = {
		.mtype = C_TO_S_TYPE,
		.msgid = fork_msgid,
	};
	msgsnd_func(msgid, &msgbuf_ctos_id, sizeof(msgbuf_ctos_id), 0);
	fprintf(stdout, "msgid :%d\n",
			msgid);
	/* comminication */
	pid_t pid;
	pid = getpid();
	struct msgbuf {
		long mtype;
		char mtext[1];
	} msgbuf = {
		.mtype  = FORK_C_TO_S,
		.mtext = '0' + pid % 10,
	};
	for ( ; ; ) {
		msgbuf.mtype = FORK_C_TO_S;
		msgsnd_func(fork_msgid, &msgbuf, sizeof(msgbuf), 0);
		fprintf(stdout, "client msgid :%d, send msgbuf.mtext:%c\n",
				msgbuf_ctos_id.msgid, msgbuf.mtext[0]);
		msgrcv_func(fork_msgid, &msgbuf, sizeof(msgbuf), FORK_S_TO_C, 0);
		fprintf(stdout, "recv msgbuf.mtext:%c\n", msgbuf.mtext[0]);
		sleep(1);
	}
	
	return 0;
}
