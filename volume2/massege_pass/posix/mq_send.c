#include <stdio.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MASSAGE_QUEUE_FILE "/temp.1234"
int main(int argc, char *argv[])
{
	if (3 > argc) {
		fprintf(stderr, "usage: ./a.out messagesize priority\n");
		exit(EXIT_FAILURE);
	}
	mqd_t mqd;
	mqd = mq_open(MASSAGE_QUEUE_FILE,
			O_RDWR | O_CREAT | O_EXCL,
			S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
			NULL);
	if (0 > mqd && EEXIST == errno) {
		fprintf(stdout, "massage queue file %s exists, continue\n",
				MASSAGE_QUEUE_FILE);
	mqd = mq_open(MASSAGE_QUEUE_FILE, O_RDWR);
	} else if (0 > mqd) {
		fprintf(stderr, "mq_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
#if 0
       mqd_t mq_send(mqd_t mqdes, const char *msg_ptr,
                      size_t msg_len, unsigned msg_prio);
#endif
	char *ptr = NULL;
	int len = atoi(argv[1]);
	int prio = atoi(argv[2]);
	ptr = malloc(len);
	mq_send(mqd, ptr, len, prio);
	free(ptr); 
	mq_close(mqd);
	
	return 0;
}
