#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define LOCK_FILE	"lock2.txt"

int main(int argc, char *argv[])
{
	if (SIG_ERR == signal(SIGCHLD, SIG_IGN)) {
		fprintf(stderr, "warning: signal set sinchld signal error :%s\n",
				strerror(errno));
	}
	/* open */
	int fd = -1;
	fd = open(LOCK_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (0 > fd) {
		fprintf(stderr, "open error :%s\n",
				strerror(errno));
		exit(EXIT_SUCCESS);
	}
	/* fork */
	int i;
	pid_t pid;
	struct flock lock = {
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
	/* wrlock */
	lock.l_type = F_WRLCK;
	if (0 > fcntl(fd, F_SETLKW, &lock)) {
		fprintf(stderr, "lock of fcntl error :%s\n",
				strerror(errno));
	}
	fprintf(stdout, "parent wrlock\n");
	for(i = 0; 10 > i; i++) {
		if (0 > (pid = fork())) {
			/* fork error */
		} else if (0 == pid) {
			/* child */
			if (3 == i) {
				/* wrlock */
				lock.l_type = F_WRLCK;
			} else {
				/* rdlock */
				lock.l_type = F_RDLCK;
			}
			fprintf(stdout, "this is process pid :%ld i:%d\n",
					(long)getpid(), i);
			/* lock */
			if (0 > fcntl(fd, F_SETLKW, &lock)) {
				fprintf(stderr, "lock of fcntl error :%s\n",
						strerror(errno));
				exit(EXIT_FAILURE);
			}
			if (lock.l_type == F_WRLCK)
				fprintf(stdout, "get wrlock\n");
			else
				fprintf(stdout, "get rdlock\n");
			sleep(2);
			/* unlock */
			fcntl(fd, F_SETLK, &unlock);
			if (lock.l_type == F_WRLCK)
				fprintf(stdout, "release wrlock\n");
			else
				fprintf(stdout, "release rdlock\n");
			/* close */
			if (0 < fd) {
				close(fd);
				fd = -1;
			}
			exit(EXIT_SUCCESS);
		} else {
			/* parent */
			usleep(100 * 1000);
		}
	}
	sleep(2);
	/* unlock */
	fcntl(fd, F_SETLK, &unlock);
	fprintf(stdout, "parent unlock\n");
	/* close */
	if (0 < fd) {
		close(fd);
		fd = -1;
	}
	for ( ; ; )
		pause();

	return 0;
}
