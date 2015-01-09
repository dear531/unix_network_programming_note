#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define FTOK_FILE "1.txt"

int main(int argc, char *argv[])
{
	/* ftok */
	key_t key;
	key = ftok(FTOK_FILE, 0xff);
	if (-1 == key) {
		fprintf(stderr, "ftok error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* msgget */
	int msgid;
	msgid = msgget(key, 0);
	if (0 > msgid) {
		fprintf(stderr, "msgget error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* msgrcv */
	struct msgbuf {
		 long mtype;
		 char mtext[1];
	} msgbuf;
	int ret;
	ret = msgrcv(msgid, &msgbuf, sizeof(msgbuf), 0, 0);
	if (0 > ret) {
		fprintf(stderr, "msgrcv error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "msgbuf.mtype:%d, msgbuf.mtext:%d\n",
			msgbuf.mtype, msgbuf.mtext[0]);

	return 0;
}
