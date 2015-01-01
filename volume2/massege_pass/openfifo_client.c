#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


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
	int n, i;
	pid_t pid;
	for (i = 0; 5 > i; i++){
		pid = fork();
		if (0 > pid) {
			/* fork error */
		} else if (0 < pid) {
			/* perant */
			sleep(1);
		} else {
			/* child */
			for ( ; ; ) {
				n = read(fd, buff, sizeof(buff));
				if (0 > n) {
					/* read error */
				} else if (0 == n) {
					/* peer close */
				} else {
					fprintf(stdout, "buff:%s, pid:%d\n",
							buff, getpid());
					sleep(5);
				}
			}
			exit(EXIT_SUCCESS);
		}
	}
	if (0 < fd) {
		close(fd);
		fd = -1;
	}

	for ( ; ; ) {
		pause();
	}
	return 0;
}
