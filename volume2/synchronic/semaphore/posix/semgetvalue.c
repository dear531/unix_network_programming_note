#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

int main(int argc, char *argv[])
{
	sem_t *sem;
	sem = sem_open(argv[1], 0);	
	if (SEM_FAILED == sem) {
		fprintf(stderr, "sem_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	int sval;
	int ret;
	ret = sem_getvalue(sem, &sval);
	if (-1 == ret) {
		fprintf(stderr, "sem_getvalue error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "semaphore value :%d\n",
			sval);
	ret = sem_close(sem);
	if (-1 == ret) {
		fprintf(stderr, "sem_close error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}
