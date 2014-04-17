#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stdout, "usage :a.out <file descriptor>\n");
		exit(EXIT_FAILURE);
	}
	int ifd;
	ifd = strtol(argv[1], NULL, 10);
	fprintf(stdout, "ifd :%d\n", ifd);

	char *file = "1.txt";
	int fd;
#define DEBUG	1
#if DEBUG
	bzero(file, 2000);
#endif
	if ((fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRWXU)) < 0)
	{
		fprintf(stdout, "open %s failed\n", file);
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
#if DEBUG
	sleep(50);
#endif
	return 0;
}
