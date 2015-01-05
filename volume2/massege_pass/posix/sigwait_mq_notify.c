/*
 * usage:
 * 1.make under current path make to ganareate -
 * execuable send and recv
 * 2. ./send 10 2 ...
 * 3. ./a.out
 * 4. test : ctrl+c 
 * 5. stop : ctrl+\
 * date:2015.1.5
 */

#include <stdio.h>
#include <signal.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int flag = 0;
void sig_usr1(int signum)
{
	flag = 1;
	return;
}
void sig_int(int signum)
{
	int errno_save = errno;
	fprintf(stdout, "caust SIGINT func\n");
	errno = errno_save;
	return;
}
#define MESSAGE_QUEUE_FILE "/temp.1234"
int main(int argc, char *argv[])
{
	struct sigevent notification;
	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGUSR1;
	sigset_t newmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	mqd_t mqd;
	/* mq_open */
	mqd = mq_open(MESSAGE_QUEUE_FILE, O_RDWR | O_NONBLOCK);
	if (0 > mqd) {
		fprintf(stdout, "mq_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* get attributer of mqd */
	struct mq_attr attr;
	mq_getattr(mqd, &attr);
	char *buff = malloc(attr.mq_msgsize);
	int prio, n;
	for ( ; ; ) {
		n = mq_receive(mqd, buff, attr.mq_msgsize, (void*)&prio);
		if (0 < n) {
			fprintf(stdout, "first receive n :%d, prio :%d\n",
				n, prio);
		} else if (0 > n && EAGAIN == errno) {
			fprintf(stdout, "mqd is empty ,waiting to send\n");
			break;
		} else {
			fprintf(stderr, "mq_receive error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	sigprocmask(SIG_BLOCK, &newmask, NULL);
	int sig = 0;
	/* polling */
	for ( ; ; ) {
		mq_notify(mqd, &notification);
		/* mq_receive */
		sigwait(&newmask, &sig);
		if (SIGUSR1 == sig) {
			for ( ; ; ) {
				n = mq_receive(mqd, buff, attr.mq_msgsize, (void*)&prio);
				if (0 < n) {
					fprintf(stdout, "n :%d, prio :%d\n",
						n, prio);
				} else if (0 > n && EAGAIN == errno) {
					fprintf(stdout, "mqd is empty ,waiting to send\n");
					sig = 0;
					break;
				} else {
					fprintf(stderr, "mq_receive error :%s\n",
							strerror(errno));
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	sigprocmask(SIG_UNBLOCK, &newmask, NULL);
	if (NULL != buff)
		free(buff);
	/* mq_close */
	mq_close(mqd);

	return 0;
}
