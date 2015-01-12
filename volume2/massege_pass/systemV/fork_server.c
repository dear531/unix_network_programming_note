#include <stdio.h>

#include "share.h"
#define FORK_C_TO_S	2L
#define FORK_S_TO_C 3L
int main(int argc, char *argv[])
{
	/* create msg */
	int msgid;
	msgid = msgget_func(MSG_KEY, IPC_CREAT);
	/* wating client to send */
	struct msgbuf_ctos_id {
		long mtype;
		int msgid;
	} msgbuf_ctos_id;
	pid_t pid;
	fprintf(stdout, "msgid :%d\n",
			msgid);
	for ( ; ; ) {
	/* pool and fork to operat new connect */
		msgrcv_func(msgid, &msgbuf_ctos_id, sizeof(msgbuf_ctos_id), C_TO_S_TYPE, 0);
		/* fork */
		if (0 > (pid = fork())) {
			/* fork error */
		} else if (0 < pid) {
			/* parent do noting*/
		} else {
			/* child */
			struct msgbuf {
				long mtype;
				char mtext[1];
			} msgbuf;

			for ( ; ; ) {
#if 0
	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
#endif
				msgrcv_func(msgbuf_ctos_id.msgid, &msgbuf, sizeof(msgbuf), FORK_C_TO_S, 0);
				msgbuf.mtype = FORK_S_TO_C;
				msgbuf.mtext[0]++;
				fprintf(stdout, "server receive from client data :%c\n",
						msgbuf.mtext[0]);
				msgsnd_func(msgbuf_ctos_id.msgid, &msgbuf, sizeof(msgbuf), 0);
			}
			exit(EXIT_SUCCESS);
		}
	}

	return 0;
}
