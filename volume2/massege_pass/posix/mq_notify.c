#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <signal.h>

#define MESSAGE_QUEUE_FILE "/temp.1234"
mqd_t mq_open_func(const char *name, int oflag)
{
	mqd_t mqd;
	mqd = mq_open(name, oflag);
	if (0 > mqd) {
		fprintf(stderr, "mq_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return mqd;
}
mqd_t mq_getattr_func(mqd_t mqdes, struct mq_attr *attr)
{
	mqd_t mqd = mq_getattr(mqdes, attr);
	if (0 > mqd) {
		fprintf(stderr, "mq_getattr error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return mqd;
}

mqd_t mq_receive_func(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio)
{
	mqd_t mqd = mq_receive(mqdes, msg_ptr, msg_len, msg_prio);
	if (0 > mqd) {
		fprintf(stderr, "mq_receive error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return mqd;
}

mqd_t mqd;
struct sigevent sigev;
struct mq_attr attr;
char *buff;
void sig_usr1(int signum)
{
	mq_notify(mqd, &sigev);
	mq_getattr_func(mqd, &attr);
	int i, prio, n;
	for (i = 0; attr.mq_curmsgs > i; i++) {
		n = mq_receive_func(mqd, buff, attr.mq_msgsize, &prio);
		fprintf(stdout, "n:%d, prio:%d\n", n, prio);
	}
	return;
}

int main(int argc, char *argv[])
{
	mqd = mq_open_func(MESSAGE_QUEUE_FILE, O_RDWR);
	signal(SIGUSR1, sig_usr1);
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	mq_notify(mqd, &sigev);
	mq_getattr_func(mqd, &attr);
	buff = malloc(attr.mq_msgsize);
	int i, prio, n;
	for (i = 0; attr.mq_curmsgs > i; i++) {
		n = mq_receive_func(mqd, buff, attr.mq_msgsize, &prio);
		fprintf(stdout, "n:%d, prio:%d\n", n, prio);
	}
	for ( ; ; )
		pause();
	free(buff);
	mq_close(mqd);
	return 0;
}
