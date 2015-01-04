#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

#define MESSAGE_QUEUE_FILE "/temp.1234"

int main(int argc, char *argv[])
{
	mqd_t mqd;
	/* oepn message queue */
	mqd = mq_open(MESSAGE_QUEUE_FILE, O_RDWR);
	if (0 > mqd) {
		fprintf(stderr, "mq_open %s failed\n",
				MESSAGE_QUEUE_FILE);
		exit(EXIT_FAILURE);
	}
	/* get attrbute message queue*/
#if 0
         struct mq_attr {
             long mq_flags;       /* Flags: 0 or O_NONBLOCK */
             long mq_maxmsg;      /* Max. # of messages on queue */
             long mq_msgsize;     /* Max. message size (bytes) */
             long mq_curmsgs;     /* # of messages currently in queue */
         };
#endif
	struct mq_attr attr;
	/* get attr of message queue */
	mq_getattr(mqd, &attr);
	/* show message queue to stdout */
	fprintf(stdout, "mq_maxmsg:%ld, mq_msgsize:%ld, mq_curmsgs:%ld\n",
			attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
	/* received message from queue */
	char buff[8192] = {0};
	int prio;
	int n;
	n = mq_receive(mqd, buff, sizeof(buff), &prio);
	fprintf(stdout, "len :%d, priority :%d\n",
			n, prio);
	/* close message queue */
	mq_close(mqd);

	return 0;
}
