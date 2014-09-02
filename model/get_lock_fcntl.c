#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	/* create tmp file */
	char tmpfile[] = "tmp.XXXXXX";
	int fd;
	fd = mkstemp(tmpfile);
	/* remove tmp file */
	unlink(tmpfile);
	/* init write lock */
	struct flock write_lock	= {
		.l_type		= F_WRLCK,
		.l_whence	= SEEK_SET,
		.l_start		= 0,
		.l_len		= 0,
	};
	struct flock unlock	= {
		.l_type		= F_UNLCK,
		.l_whence	= SEEK_SET,
		.l_start		= 0,
		.l_len		= 0,
	};
	struct flock get_lock;
	/* create mutil process */
	pid_t pid;
	if (0 > (pid = fork())) {
		/* pid < 0, error */
	} else if (0 == pid) {
		/* pid is 0, child process */
		sleep(1);
		memset(&get_lock, 0x00, sizeof(get_lock));
		fcntl(fd, F_GETLK, &get_lock);
		fprintf(stdout, "get_lock.l_pid :%d\n",
				get_lock.l_pid);
		/* child exit, prevent pool fork */
		exit(EXIT_SUCCESS);
	} else {
		/* pid > 0, perent */
		fprintf(stdout, "perent pid :%d\n",
				getpid());
		fcntl(fd, F_SETLK, &write_lock);
		sleep(2);
		fcntl(fd, F_SETLK, &unlock);
		
	}
	return 0;
}
