#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define SEM_FILE	"file.sem"
#define LOOP_NUM	50
int main(int argc, char *argv[])
{
	sem_t *sem;
	pid_t pid;
	int count = 0;
	int i;
	/* create file */
	open(SEM_FILE, O_CREAT, S_IRUSR | S_IWUSR);
	/* semaphore create */
	sem = sem_open(SEM_FILE, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (SEM_FAILED == sem) {
		fprintf(stdout, "sem_open error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* semaphore unlink */
	sem_unlink(SEM_FILE);
	/* unlink file */
	unlink(SEM_FILE);
	setbuf(stdout, NULL);
	/* fork */
	pid = fork();
	if (-1 == pid) {
		fprintf(stderr, "fork error :%s\n",
				strerror(errno));
		goto failure;
	} else if (0 < pid) {
	/* parent */
			sem_wait(sem);
			for (i = 0; LOOP_NUM > i; i++) {
				fprintf(stdout, "parend count :%d\n",
						count++);
			}
			sem_post(sem);
	} else {
	/* child */
			sem_wait(sem);
			for (i = 0; LOOP_NUM > i; i++) {
				fprintf(stdout, "child count :%d\n",
						count++);
			}
			sem_post(sem);
			exit(EXIT_SUCCESS);
	}
	return 0;
failure:
	sem_unlink(SEM_FILE);
	unlink(SEM_FILE);
}
