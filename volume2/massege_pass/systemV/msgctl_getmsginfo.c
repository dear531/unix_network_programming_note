#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_KEY	1234L

int main(int argc, char *argv[])
{
	/* msgget */
	int msgid;
	msgid = msgget(MSG_KEY, IPC_CREAT);
	if (0 > msgid) {
		fprintf(stderr, "msgget error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* msgctl */
	struct msqid_ds msginfobuf;
	int ret;
	ret = msgctl(msgid, MSG_INFO, &msginfobuf);
	if (0 > ret) {
		fprintf(stderr, "msgctl error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
#if 0
           struct msqid_ds {
               struct ipc_perm msg_perm;     /* Ownership and permissions
               time_t         msg_stime;    /* Time of last msgsnd() */
               time_t         msg_rtime;    /* Time of last msgrcv() */
               time_t         msg_ctime;    /* Time of last change */
               unsigned long  __msg_cbytes; /* Current number of bytes in
                                               queue (non-standard) */
               msgqnum_t      msg_qnum;     /* Current number of messages
                                               in queue */
               msglen_t       msg_qbytes;   /* Maximum number of bytes
                                               allowed in queue */
               pid_t          msg_lspid;    /* PID of last msgsnd() */
               pid_t          msg_lrpid;    /* PID of last msgrcv() */
           };
#endif
	fprintf(stdout, "index :%d, msg_stime :%d, __msg_cbytes:%ld,\
 msg_qnum:%d, msg_qbytes:%d, msg_lspid:%d, msg_lspid:%d\n",
		ret, msginfobuf.msg_stime, msginfobuf.__msg_cbytes,
msginfobuf.msg_qnum, msginfobuf.msg_qbytes, msginfobuf.msg_lspid);
	/* show */
	return 0;
}
