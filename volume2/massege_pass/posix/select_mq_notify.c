#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <mqueue.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#define MESSAGE_QUEUE_FILE "/temp.1234"
int pfd[2] = {0};
void sig_usr1(int signum)
{
	/* write fd of pipe */
	write(pfd[1], "", sizeof(""));
	return;
}
int main(void)
{
	/* mq_open */
	mqd_t mqd;
	mqd = mq_open(MESSAGE_QUEUE_FILE, O_RDWR);
	if (0 > mqd) {
		fprintf(stderr, "mq_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* set handle function of sigusr1 */
	signal(SIGUSR1, sig_usr1);
	/* create pipe */
	pipe(pfd);
	/* mq_notify */
	struct sigevent notify;
	notify.sigev_notify = SIGEV_SIGNAL;
	notify.sigev_signo = SIGUSR1;
	mq_notify(mqd, &notify);
	/* mq_getattr */
	struct mq_attr attr;
	mq_getattr(mqd, &attr);
	/* set buff */
	char *buff = malloc(attr.mq_msgsize);
	/* select */
	fd_set rset;
	int n, readn, prio;
	for ( ; ; ) {
		FD_ZERO(&rset);
		FD_SET(pfd[0], &rset);
		n = select(pfd[0] + 1, &rset, NULL, NULL, NULL);
		mq_notify(mqd, &notify);
		if (0 < n) {
			/* pfd readable */
			if (FD_ISSET(pfd[0], &rset)) {
				memset(buff, 0x00, attr.mq_msgsize);
				read(pfd[0], buff, attr.mq_msgsize);
				fprintf(stdout, "buff:%s\n", buff);
				memset(buff, 0x00, attr.mq_msgsize);
				readn = mq_receive(mqd, buff, attr.mq_msgsize, &prio);
				fprintf(stdout, "n:%d, prio:%d\n",
						readn, prio);
			}
		} else {
			/* error */
		}
	}
	/* free buff */
	if (NULL != buff)
		free(buff);
	/* mq_close */
	if (0 < mqd)
		mq_close(mqd);
	return 0;
}
