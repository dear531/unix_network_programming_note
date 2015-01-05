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
	/* init signal set */
	sigset_t zeromask;
	sigset_t newmask;
	sigset_t oldmask;
	struct sigevent notification;
	sigfillset(&zeromask);
	sigdelset(&zeromask, SIGUSR1);
	sigdelset(&zeromask, SIGQUIT);
	sigemptyset(&newmask);
	sigemptyset(&oldmask);
	sigaddset(&newmask, SIGUSR1);
	mqd_t mqd;
	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGUSR1;
	/* mq_open */
	mqd = mq_open(MESSAGE_QUEUE_FILE, O_RDWR | O_NONBLOCK);
	if (0 > mqd) {
		fprintf(stdout, "mq_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	signal(SIGUSR1, sig_usr1);
	signal(SIGINT, sig_int);
	/* save origin signal mask */
	sigprocmask(SIG_BLOCK, NULL, &oldmask);
	mq_notify(mqd, &notification);
	/* get attributer of mqd */
	struct mq_attr attr;
	mq_getattr(mqd, &attr);
	char *buff = malloc(attr.mq_msgsize);
	int prio, n;
	for ( ; ; ) {
		n = mq_receive(mqd, buff, attr.mq_msgsize, &prio);
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
	/* polling */
	for ( ; ; ) {
		sigprocmask(SIG_BLOCK, &newmask, NULL);
		/* suspend all signal */
		while (0 == flag) {
			sigsuspend(&zeromask);
		}
		flag = 0;
		mq_notify(mqd, &notification);
		/* mq_receive */
		for ( ; ; ) {
			n = mq_receive(mqd, buff, attr.mq_msgsize, &prio);
			if (0 < n) {
				fprintf(stdout, "n :%d, prio :%d\n",
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
		sigprocmask(SIG_UNBLOCK, &newmask, NULL);
	}
	/* restore origin signal mask */
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	if (NULL != buff)
		free(buff);
	/* mq_close */
	mq_close(mqd);

	return 0;
}
