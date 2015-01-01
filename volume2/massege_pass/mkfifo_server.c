#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPEFILE "1.fifo"

int main(int argc, char *argv[])
{
	int ret;
	int fd;
again:
	ret = mkfifo(PIPEFILE, 755);
	if (0 > ret) {
		fprintf(stderr, "mkfifo error:%s\n",
				strerror(errno));
		unlink(PIPEFILE);
		goto again;
	}
	fd = open(PIPEFILE, O_WRONLY);
	if (0 > fd) {
		fprintf(stdout, "open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	char buff[sizeof("this is server") + 1] = "this is server";
	int n, i;
	for (i = 0; ;i++) {
		buff[sizeof("this is server") - 1] = '0' + i;
		n = write(fd, buff, sizeof(buff));
		if (0 > n) {
			/* write error */
			exit(EXIT_SUCCESS);
		} else if (0 == n) {
			/* can not write */
		} else {
			/* write success */
			fprintf(stdout, "buff:%s\n",
					buff);
			sleep(1);
		}
	}
	if (-1 != fd) {
		close(fd);
		fd = -1;
	}
	unlink(PIPEFILE);
	return 0;
}
