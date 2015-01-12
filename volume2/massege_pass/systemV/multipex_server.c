#include <stdio.h>

#include "share.h"
#define S_TO_C_CONTEXT	"1234567890"
int main(int argc, char *argv[])
{
	int msgid;
	msgid = msgget_func(MSG_KEY, IPC_CREAT);
	struct msgbuf_ctos {
		long mtype;
		int pid;
	} msgbuf_ctos;
	struct msgbuf_stoc {
		long mtype;
		char mtext[20];
	} msgbuf_stoc;
	for ( ; ; ) {
		msgrcv_func(msgid, &msgbuf_ctos, sizeof(msgbuf_ctos), C_TO_S_TYPE, 0);
		fprintf(stdout, "received form pid :%d client data\n",
				msgbuf_ctos.pid);
		msgbuf_stoc.mtype = (int)msgbuf_ctos.pid;
		bcopy(S_TO_C_CONTEXT, msgbuf_stoc.mtext, sizeof(S_TO_C_CONTEXT) - 1);
		msgbuf_stoc.mtext[sizeof(S_TO_C_CONTEXT) - 1 - 1] = '0' + msgbuf_ctos.pid % 10;
		msgsnd_func(msgid, &msgbuf_stoc, sizeof(msgbuf_stoc), 0);
	}
	return 0;
}
