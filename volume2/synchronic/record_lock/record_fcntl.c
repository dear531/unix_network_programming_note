#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>


#define RECORD_FILE "1.txt"

inline void init_region(struct flock *lock)
{
	lock->l_whence = SEEK_SET;
	lock->l_start = 0;
	lock->l_len = 0;
}

int operat_lock(int fd, short l_type, struct flock *lock)
{
	lock->l_type = l_type;
	return fcntl(fd, F_SETLKW, lock);
}

int record_lock(int fd)
{
	struct flock wrlock;
	init_region(&wrlock);
	return operat_lock(fd, F_WRLCK, &wrlock);
}

int record_unlock(int fd)
{
	struct flock unlock;
	init_region(&unlock);
	return operat_lock(fd, F_UNLCK, &unlock);
}

int main(int argc, char *argv[])
{
	if (SIG_ERR == signal(SIGCHLD, SIG_IGN)) {
		fprintf(stderr, "wring: set signal SIGCHLD as SIG_IGN failed :%s\n",
				strerror(errno));
	}
	int fd;
	fd = open(RECORD_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	int i;
	pid_t pid;
	for (i = 0; 3 > i; i++) {
		if (0 > (pid = fork())) {
			/* fork error */
		} else if (0 == pid) {
			/* child process */
			/* do */
			pid_t chld_pid;
			chld_pid = getpid();
			record_lock(fd);
			fprintf(stdout, "process %ld start\n", (long)chld_pid);
			char buff[1024] = {0};
			fprintf(stdout, "");
			sprintf(buff, "start %ld\n", (long)chld_pid);
			write(fd, buff, strlen(buff));
			for (i = 0; 10 > i; i++) {
				memset(buff, 0x00, strlen(buff));
				sprintf(buff, "process %ld write %d\n", chld_pid, i);
				write(fd, buff, strlen(buff));
				usleep(100 * 1000);
			}
			memset(buff, 0x00, strlen(buff));
			sprintf(buff, "close process %ld\n", (long)chld_pid);
			write(fd, buff, strlen(buff));
			record_unlock(fd);
			close(fd);
			fprintf(stdout, "process %ld exit\n", (long)chld_pid);
			/* exit */
			exit(EXIT_SUCCESS);
		} else {
			/* parent process */
		}
	}
	for ( ; ; )
		pause();
	if (0 < fd) {
		close(fd);
		fd = -1;
	}
	return 0;
}
