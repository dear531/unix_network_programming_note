#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MQ_KEY1	1234L
#define MQ_KEY2	5678L
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
	ssize_t  rcvn;
	if (0 > (rcvn = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg))) {
		fprintf(stderr, "msgrcv error %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return rcvn;
}
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
void client(int read, int write)
{
#if 0
	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
#endif
	struct msgbuf {
		 long mtype;     /* message type, must be > 0 */
		 char mtext[10];  /* message data */
	} msgbuf = {.mtype = 1, "01234567"};
	int i;
	for (msgbuf.mtext[8] = '0', i = 0; ; i++, msgbuf.mtext[8] = '0' + i) {
		msgsnd_func(write, &msgbuf, sizeof(msgbuf), 0);
		fprintf(stdout, "send to server msgbuf :%s\n",
				msgbuf.mtext);
		msgrcv_func(read, &msgbuf, sizeof(msgbuf), 0, 0);
		fprintf(stdout, "recv from server msgbuf :%s\n",
				msgbuf.mtext);
		sleep(1);
	}
	return;
}
int main(int argc, char *argv[])
{
	int msgread, msgwrite;
	msgread = msgget_func(MQ_KEY2, 0);	
	msgwrite = msgget_func(MQ_KEY1, 0);	
	client(msgread, msgwrite);

	return 0;
}
