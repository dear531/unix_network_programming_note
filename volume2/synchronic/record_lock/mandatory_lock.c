/*
 * step 1: $ touch mandatory.txt 
 * step 2: $ chmod g+s,g-x mandatory.txt
 * step 3: cc mandatory_lock.c and run a.out
 * step 4: new terminal input:
 * $ echo "" > mandatory.txt 
 * bash: mandatory.txt: Resource temporarily unavailable
 * or
 * $ cat mandatory.txt (block util release mandatory lock)
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


#define MANDATORY_FILE "mandatory.txt"

int main(int argc, char *argv[])
{
	/* open */
	int fd = -1;
	fd = open(MANDATORY_FILE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	if (0 > fd) {
		fprintf(stderr, "open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* give sticky permiss */
	/* lock */
#if 0
         struct flock {
             ...
             short l_type;    /* Type of lock: F_RDLCK,
                                 F_WRLCK, F_UNLCK */
             short l_whence;  /* How to interpret l_start:
                                 SEEK_SET, SEEK_CUR, SEEK_END */
             off_t l_start;   /* Starting offset for lock */
             off_t l_len;     /* Number of bytes to lock */
             pid_t l_pid;     /* PID of process blocking our lock
                                 (F_GETLK only) */
             ...
         };
#endif
	struct flock wrlock = {
		.l_type = F_WRLCK,
		.l_whence = SEEK_SET,
		.l_start = 0,
		.l_len = 0,
	};
	if (0 > fcntl(fd, F_SETLK, &wrlock)) {
		fprintf(stderr, "fcntl set wrlock error\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* sleep 10 second */
	sleep(20);
	/* unlock */
	struct flock unlock = {
		.l_type = F_UNLCK,
		.l_whence = SEEK_SET,
		.l_start = 0,
		.l_len = 0,
	};
	if (0 > fcntl(fd, F_SETLK, &unlock)) {
		fprintf(stderr, "fcntl unlock error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* close */
	if (0 < fd) {
		close(fd);
		fd = -1;
	}

	return 0;
}
