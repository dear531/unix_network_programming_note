/*
 * usage : make and run ./mma_not_inherit
 * and open new terminal run ./mmap_not_inherit
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>


struct share {
	sem_t sem;
	int count;
};
int lockfd = -1;
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
struct flock lock = {	
	.l_type = F_WRLCK,
	.l_whence = SEEK_SET,
	.l_start = 0,
	.l_len = 0,
};
struct flock unlock = {	
	.l_type = F_WRLCK,
	.l_whence = F_UNLCK,
	.l_start = 0,
	.l_len = 0,
};
#define SHARE_FILE	"file.share"
#define LOCK_FILE	"file.lock"
struct share *ps = NULL;
void sig_int(int signum)
{
	if (SIGINT != signum)
		return;
	int errsv;
	errsv = errno;
	fcntl(lockfd, F_SETLK, &unlock);
	unlink(LOCK_FILE);
	unlink(SHARE_FILE);
	errno = errsv;
	exit(EXIT_SUCCESS);
}
int main(int argc, char *argv[])
{
	int fd = -1;
	int ret = EXIT_SUCCESS;
	int prefrom;
	int init = 0;
	int share = 0;
	if (SIG_ERR == signal(SIGINT, sig_int)) {
		fprintf(stderr, "warning signal set error\n");
	}
	lockfd = open(LOCK_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (-1 == lockfd) {
		fprintf(stderr, "line :%d open error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	}
	/* open file */
	fd = open(SHARE_FILE, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if (-1 == fd && EEXIST == errno) {
		fd = open(SHARE_FILE,  O_RDWR);
	} else if (-1 == fd) {
		fprintf(stderr, "line :%d open error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	} else {
		share = 1;
	}
	if (-1 == fd) {
		fprintf(stderr, "line %d open error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	}
	lseek(fd, sizeof(*ps), 0);
	write(fd, "0", sizeof("0"));
	/* mmap */
	ps = mmap(NULL, sizeof(*ps), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (MAP_FAILED == ps) {
		fprintf(stderr, "mmap error :%s\n",
				strerror(errno));
		goto failure;
	}
	prefrom = fcntl(lockfd, F_SETLK, &lock);
	if (-1 == prefrom && EAGAIN == errno) {
		goto continue1;
	} else if (-1 == prefrom) {
		goto failure;
	} else {
	/* initialized semaphore */
		if (-1 == sem_init(&ps->sem, -1, 1)) {
			fprintf(stderr, "sem_init error :%s\n",
					strerror(errno));
			goto failure;
		}
		ps->count = 0;
		init = 1;
	}
continue1:
	for ( ; ; ) {
		sem_wait(&ps->sem);
		fprintf(stdout, "ps->count :%d\n",
				ps->count++);
		sem_post(&ps->sem);
		sleep(2);
	}
finish:
	/* close fd */
	if (-1 != fd) {
		close(fd);
		fd = -1;
	}
	if (1 == init) {
		fcntl(lockfd, F_SETLK, &unlock);
		unlink(LOCK_FILE);
	}
	if (1 == share) {
		unlink(SHARE_FILE);
	}
	/* munmap */
	munmap(ps, sizeof(*ps));
	exit(ret);
failure:
	ret = EXIT_FAILURE;
	goto finish;
}
