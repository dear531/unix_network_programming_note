#include <stdio.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MASSAGE_QUEUE_NAME "/temp.1234"
int main(int argc, char *argv[])
{
	mqd_t mqd;
	struct mq_attr mqattr;
             mqattr.mq_flags = O_NONBLOCK;
             mqattr.mq_maxmsg = 10 * 2;
             mqattr.mq_msgsize = 8192 * 2;
             mqattr.mq_curmsgs = 2;
	fprintf(stdout, "flags:%ld,maxmsg:%ld,msgsize:%ld,curmsgs:%ld\n",
			mqattr.mq_flags, mqattr.mq_maxmsg, mqattr.mq_msgsize, mqattr.mq_curmsgs);
again:
	mqd = mq_open(MASSAGE_QUEUE_NAME,
			O_RDWR | O_CREAT | O_EXCL,
			S_IRWXU,
			&mqattr);
	if (0 > mqd && EEXIST == errno) {
		mq_unlink(MASSAGE_QUEUE_NAME);
		fprintf(stdout, "unlink exits :%s massage queue, retry create\n", MASSAGE_QUEUE_NAME);
		goto again;
	} if (0 > mqd) {
		fprintf(stderr, "mq_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	mq_getattr(mqd, &mqattr);
#if 0
         struct mq_attr {
             long mq_flags;       /* Flags: 0 or O_NONBLOCK */
             long mq_maxmsg;      /* Max. # of messages on queue */
             long mq_msgsize;     /* Max. message size (bytes) */
             long mq_curmsgs;     /* # of messages currently in queue */
         };
#endif
	fprintf(stdout, "flags:%ld,maxmsg:%ld,msgsize:%ld,curmsgs:%ld\n",
			mqattr.mq_flags, mqattr.mq_maxmsg, mqattr.mq_msgsize, mqattr.mq_curmsgs);
	struct mq_attr newattr;
	newattr.mq_flags = 0;
	mq_setattr(mqd, &newattr, NULL);
	mq_getattr(mqd, &mqattr);
	fprintf(stdout, "set new attribut flags:%ld,maxmsg:%ld,msgsize:%ld,curmsgs:%ld\n",
			mqattr.mq_flags, mqattr.mq_maxmsg, mqattr.mq_msgsize, mqattr.mq_curmsgs);
	fprintf(stdout, "O_NONBLOCK:%d\n", O_NONBLOCK);
	mq_close(mqd);
	return 0;
}
