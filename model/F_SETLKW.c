#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char tmpfile[] = "tmp.XXXXXX";
	int fd;
	/* create file discriptor */
	fd = mkstemp(tmpfile);
	/* rm file */
	unlink(tmpfile);
	struct flock lock = {
		.l_type = F_WRLCK,
		.l_whence = SEEK_SET,
		.l_start = 0,
		.l_len = 0,
	};
	struct flock unlock = {
		.l_type = F_UNLCK,
		.l_whence = SEEK_SET,
		.l_start = 0,
		.l_len = 0,
	};
	/* fork */
	pid_t pid;
	if ((pid = fork()) < 0) {
	/* error */
		fprintf(stdout, "fork error :%s\n",
			strerror(errno));
	} else if (pid == 0) {
		sleep(2);
		fcntl(fd, F_SETLKW, &lock);
		fprintf(stdout, "child process get fd\n");
	/* child process get lock */
	} else {
	/* perent lock */
		fprintf(stdout, "perent lock\n");
		fcntl(fd, F_SETLKW, &lock);
		sleep(10);
		fprintf(stdout, "perent unlock\n");
		fcntl(fd, F_SETLK, &unlock);
	}
	/* close fd */
	close(fd);
	return 0;
}
