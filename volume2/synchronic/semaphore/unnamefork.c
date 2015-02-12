#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>

void show_value(sem_t *sem)
{
	int sval;
	if (-1 == sem_getvalue(sem, &sval)) {
		fprintf(stderr, "sem_getvalue error :%s\n",
				strerror(errno));
	} else {
		fprintf(stdout, "pid :%ld value :%d\n",
				(long)getpid(), sval);
	}
}

int main(int argc, char *argv[])
{
	sem_t sem;
	/* semaphore initialized */
	int ret;
	if (-1 == sem_init(&sem, 1, 0)) {
		fprintf(stderr, "sem_init error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* fork  */
	pid_t pid;
	if (0 > (pid = fork())) {
		/* fork error */
		fprintf(stderr, "fork error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	} else if (0 == pid) {
		/* child */
		for ( ; ; ) {
			show_value(&sem);
			if (-1 == sem_wait(&sem)) {
				fprintf(stderr, "sem_wait error :%s\n",
						strerror(errno));
				exit(EXIT_FAILURE);
			}
			show_value(&sem);
			sleep(1);
		}
	} else {
		/* parent */
		for ( ; ; ) {
			show_value(&sem);
			if (-1 == sem_post(&sem)) {
				fprintf(stderr, "sem_wait error :%s\n",
						strerror(errno));
				exit(EXIT_FAILURE);
			}
			show_value(&sem);
			sleep(1);
		}
	}
	/* semaphore destroyed */
	if (-1 == sem_destroy(&sem)) {
		fprintf(stderr, "sem_destroy error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
