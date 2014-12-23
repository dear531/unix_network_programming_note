#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char tmpfile[] = "tmp.XXXXXX";
	int fd;
	/* create file discptor */
	fd = mkstemp(tmpfile);
	/* remove temporary, but still open file */
	unlink(tmpfile);
	/* init lock and unlock */
	struct flock read_lock	= {
		.l_type		= F_RDLCK,
		.l_whence	= SEEK_SET,
		.l_start	= 0,
		.l_len		= 0,
	};
	struct flock write_lock	= {
		.l_type		= F_WRLCK,
		.l_whence	= SEEK_SET,
		.l_start	= 0,
		.l_len		= 0,
	};
	struct flock unlock	= {
		.l_type		= F_UNLCK,
		.l_whence	= SEEK_SET,
		.l_start	= 0,
		.l_len		= 0,
	};

	pid_t pid;
	int i;
	int ret;
	for (i = 0; i < 6; i++) {
	/* create mutil process */
		if ((pid = fork()) < 0) {
			/* error */
		} else if (pid == 0) {
			/* child process */
			while (1) {
				/* read_lock */
				fprintf(stdout, "child pid :%d, i :%d ", getpid(), i);
				if (0 > (ret = fcntl(fd, F_SETLK, &read_lock))
					&& EAGAIN == errno) {
					fprintf(stdout, "read again\n");
				} else if (0 > ret) {
					fprintf(stdout, "read error :%s\n",
							strerror(errno));
				} else {
					fprintf(stdout, "read get lock\n");
				}
				/* unlokc */
				fcntl(fd, F_SETLK, &unlock);
				sleep(1);
			}
			exit(EXIT_SUCCESS);
		} else {
			/* perent process */
		}
	}
	while (1) {
		/* write lock */
		fprintf(stdout, "perent process ");
		if ((ret = fcntl(fd, F_SETLK, &write_lock)) < 0
				&& errno == EAGAIN) {
			fprintf(stdout, "write again\n");
		} else if (ret < 0) {
			fprintf(stdout, "write lock error %s\n",
					strerror(errno));
		} else {
			fprintf(stdout, "perent get lock\n");
		}
		/* unlokc */
		fcntl(fd, F_SETLK, &unlock);
		sleep(1);
	}

	return 0;
}
