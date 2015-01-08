#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int argc, char *argv[])
{
	int msgid;
	msgid = msgget(IPC_PRIVATE, IPC_CREAT);
#if 0
            struct msgbuf {
                 long mtype;     /* message type, must be > 0 */
                 char mtext[1];  /* message data */
            };

	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
#endif
	system("ipcs -q");
	struct msgbuf {
		 long mtype;
		 char mtext[1];
	} msgp;
	msgp.mtype = 1;
	msgp.mtext[0] = 'A';
	msgsnd(msgid, &msgp, sizeof(msgp), 0);
	system("ipcs -q");
#if 0
	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
#endif
	struct msgbuf rcvmsgp;
	msgrcv(msgid, &rcvmsgp, sizeof(rcvmsgp), 0, 0);
	fprintf(stdout, "rcvmsgp.mtype:%d, rcvmsgp.mtext:%d\n",
			rcvmsgp.mtype, rcvmsgp.mtext[0]);
	system("ipcs -q");
	msgctl(msgid, IPC_RMID, NULL);
	system("ipcs -q");
	return 0;
}
