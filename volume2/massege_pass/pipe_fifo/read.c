#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	fd = open("1.txt", O_WRONLY | O_CREAT);
	if (0 > fd) {
		fprintf(stderr, "open 1.txt error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	write(fd, "1", sizeof("1"));
	if (0 < fd) {
		close(fd);
		fd = -1;
	}
	return 0;
}
