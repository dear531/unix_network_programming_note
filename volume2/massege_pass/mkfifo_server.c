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
	char buff[] = "this is server";
	write(fd, buff, sizeof(buff));
	close(fd);
	unlink(PIPEFILE);
	return 0;
}
