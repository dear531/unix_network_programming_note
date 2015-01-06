#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

#define MESSAGE_QUEUE_FILE "/temp.1234"

mqd_t mqd = -1;
struct sigevent sigev;
/* pthread receive */
void notify_thread(union sigval arg)
{
	/* get attributer for message size */
#if 0
         struct mq_attr {
             long mq_flags;       /* Flags: 0 or O_NONBLOCK */
             long mq_maxmsg;      /* Max. # of messages on queue */
             long mq_msgsize;     /* Max. message size (bytes) */
             long mq_curmsgs;     /* # of messages currently in queue */
         };
#endif
	struct mq_attr attr;
	mq_getattr(mqd, &attr);
	/* create buff */
	char *buff = malloc(attr.mq_msgsize);
	/* retry notify */
	mq_notify(mqd, &sigev);
	/* mq_receive */
	int prio, n;
	fprintf(stdout, "sigev.sigev_value:%d,arg.sival_int:%d\n",
			sigev.sigev_value.sival_int++, arg.sival_int++);
	for ( ; ; ) {
		n = mq_receive(mqd, buff, attr.mq_msgsize, &prio);
		if (0 > n && EAGAIN != errno) {
			/* error receive */
			fprintf(stderr, "mq_receive error :%s\n",
					strerror(errno));
			pthread_exit(NULL);
		} else if (0 > n) {
			/* message queue is empty */
			fprintf(stdout, "message queue is empty\n");
			break;
		} else {
			fprintf(stdout, "n : %d, prio :%d\n",
					n, prio);
		}
	}
	/* free buff */
	if (NULL != buff)
		free(buff);
	/* exit of the pthread */
	pthread_exit(NULL);
}
/* main */
int main(int argc, char *argv[])
{
	/* mq_open */
	mqd = mq_open(MESSAGE_QUEUE_FILE, O_RDWR | O_NONBLOCK);
	if (0 > mqd) {
		fprintf(stderr, "mq_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* mq_notify for set pthread handle */
#if 0
         struct sigevent {
             int    sigev_notify;      /* Notification method */
             int    sigev_signo;       /* Notification signal */
             union sigval sigev_value; /* Data passed with notification */
             void (*sigev_notify_function) (union sigval);
                                       /* Function for thread notification */
             void  *sigev_notify_attributes;
                                       /* Thread function attributes */
         };
         union sigval {                /* Data passed with notification */
             int     sival_int;        /* Integer value */
             void   *sival_ptr;        /* Pointer value */
         };

#endif
	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_notify_function = notify_thread;
	sigev.sigev_value.sival_int = 1;
	sigev.sigev_notify_attributes = NULL;
	mq_notify(mqd, &sigev);
	/* sleep */
	for ( ; ; )
		pause();
	/* mq_close */
	if (0 < mqd)
		mq_close(mqd);
	return 0;
}

