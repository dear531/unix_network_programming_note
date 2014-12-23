#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	struct flock lock_file = {
		.l_type = F_WRLCK,
		.l_whence = SEEK_SET,
		.l_start = 0,
		.l_len = 0,
	};
	struct flock unlock_file = {
		.l_type = F_UNLCK,
		.l_whence = SEEK_SET,
		.l_start = 0,
		.l_len = 0,
	};
	char tmpfile[] = "tmp.XXXXXX";
	int fd;
	int ret;
	fd = mkstemp(tmpfile);
	pid_t pid;
	if ((pid = fork()) < 0) {
	/* error */
	} else if (pid == 0) {
	/* child */
		sleep(2);
again:
		if ((ret = fcntl(fd, F_SETLK, &lock_file)) < 0
				&& errno == EAGAIN) {
			sleep(1);
			fprintf(stdout, "again\n");
			goto again;
		} else if (ret < 0) {
			fprintf(stdout, "fcntl error :%s\n", strerror(errno));
		} else {
			fprintf(stdout, "acquire success\n");
		}
		close(fd);
		exit(EXIT_SUCCESS);
	} else {
	/* perent */
		ret = fcntl(fd, F_SETLKW, &lock_file);
		fprintf(stdout, "ret :%d\n", ret);

		sleep(10);
		fcntl(fd, F_SETLK, &unlock_file);
		/* unlock */
	}

	pause();

	return 0;
}
