#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>



#define SEM_FILE	"lock_sem"
sem_t *sem;
void sig_int(int signum)
{
	if (SIGINT != signum) {
		return;
	}
	/* sem_unlink */
	if (-1 == sem_unlink(SEM_FILE)) {
		fprintf(stderr, "sem_unlink error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
	
}
int main(int argc, char *argv[])
{
	/* sigint function for signal */
	if (SIG_ERR == signal(SIGINT, sig_int)) {
		fprintf(stderr, "signal error :%s\n",
				strerror(errno));
	}
	/* sem_open */
	sem = sem_open(SEM_FILE, O_CREAT | O_RDWR,
			S_IRUSR | S_IWUSR, 1);
	if (SEM_FAILED == sem) {
		fprintf(stderr, "sem_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* sem_wait */
	int ret;
	ret = sem_trywait(sem);
	if (-1 == ret && EAGAIN == errno) {
		fprintf(stdout, "same process runing\n");
		exit(EXIT_SUCCESS);
	} else if (-1 == ret) {
		fprintf(stderr, "sem_trywait error :%s\n",
				strerror(errno));
	}
	for ( ; ; ) {
		write(STDOUT_FILENO, ".", sizeof("."));
		sleep(1);
	}
	/* sem_close */
	if (-1 == sem_close(sem)) {
		fprintf(stderr, "sem_close error :%s\n",
				strerror(errno));
	}
	/* sem_unlink */
	if (-1 == sem_unlink(SEM_FILE)) {
		fprintf(stderr, "sem_unlink error :%s\n",
				strerror(errno));
	}
	
	return 0;
}
