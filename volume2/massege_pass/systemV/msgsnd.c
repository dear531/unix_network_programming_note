#include <stdio.h>
# include <sys/types.h>
# include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define FTOK_FILE "1.txt"

int main(int argc, char *argv[])
{
	key_t key;
	key = ftok(FTOK_FILE, 0xff);
	if (-1 == key) {
		fprintf(stderr, "ftok error: %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	int msgid;
	msgid = msgget(key, 0);
	if (-1 == msgid) {
		fprintf(stderr, "msgget error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct msgbuf {
		 long mtype;
		 char mtext[1];
	} msgbuf = {
		.mtype = 1,
		.mtext[0] = 2,
	};
	int ret;
	ret = msgsnd(msgid, &msgbuf, sizeof(msgbuf), 0);
	if (0 > ret) {
		fprintf(stderr, "msgsnd error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}
