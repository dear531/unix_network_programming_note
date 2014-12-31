#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PIPEFILE "1.fifo"
int main(int argc, char *argv[])
{
	int fd;
	fd = open(PIPEFILE, O_RDONLY);
	if (0 > fd) {
		fprintf(stdout, "open error :%s\n",
				strerror(errno));
	}
	char buff[1024] = {0};
	int n;
	n = read(fd, buff, sizeof(buff));
	if (0 > n) {
		/* read error */
	} else if (0 == n) {
		/* peer close */
	} else {
		fprintf(stdout, "buff:%s\n",
				buff);
	}
	if (0 < fd)
		close(fd);
	return 0;
}
