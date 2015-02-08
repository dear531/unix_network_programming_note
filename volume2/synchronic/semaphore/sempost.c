#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
void show_value(sem_t *sem)
{
#if 0
       int sem_getvalue(sem_t *sem, int *sval);
#endif
	int ret, sval;
	ret = sem_getvalue(sem, &sval);
	if (-1 == ret) {
		fprintf(stderr, "sem_getvalue error\n");
		goto finish;
	}
	fprintf(stdout, "semaphore value :%d\n",
			sval);
finish:
	return;
}

int main(int argc, char *argv[])
{
	/* check command-line */
	if (2 != argc) {
		fprintf(stderr, "usage : sempost name");
		exit(EXIT_FAILURE);
	}
	/* sem_open */
	sem_t *sem;
	sem = sem_open(argv[1], 0);
	if (SEM_FAILED == sem) {
		fprintf(stderr, "sem_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* show value */
	show_value(sem);
	int ret;
	/* sem_post */
	ret = sem_post(sem);
	if (-1 == ret) {
		fprintf(stderr, "sem_post error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* show value */
	show_value(sem);
	/* sem_close */
	ret = sem_close(sem);
	if (-1 == ret) {
		fprintf(stderr, "sem_close error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
