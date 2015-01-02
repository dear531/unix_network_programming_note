#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
#if 0
   mqd_t mq_open(const char *name, int oflag, mode_t mode,
                     struct mq_attr *attr);
#endif
	mqd_t mqt;
	mqt	= mq_open("/temp.1234", O_RDWR | O_CREAT, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP, NULL);
	if (0 > mqt) {
		fprintf(stderr, "mq_open error :%s\n",
				strerror(errno));
	}
	for ( ; ; )
		pause();
	mq_close(mqt);
	return 0;
}
