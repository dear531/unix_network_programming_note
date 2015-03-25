#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>

struct share {
	sem_t sem;
	int count;
};
#define SEMFILE	"/dev/zero"
int main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;
	pid_t pid;
	struct share *pshare = NULL, data;
	pshare = &data;
	int semfd = -1;
	int i;
	/* create file semaphore and count */
	semfd = open(SEMFILE, O_RDWR);
	if (-1 == semfd) {
		fprintf(stderr, "line %d open error %s",
				__LINE__, strerror(errno));
		goto failure;
	}
	write(semfd, &data, sizeof(data));
	/* mmap for semaphore and count */
	pshare = mmap(NULL, sizeof(*pshare), PROT_READ | PROT_WRITE,
			MAP_SHARED, semfd, 0);
	if (MAP_FAILED == pshare) {
		fprintf(stderr, "line %d mmap error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	}
	/* semaphore initialized */
	if (-1 == sem_init(&pshare->sem, -1, 1)) {
		fprintf(stderr, "sem_init error :%s\n",
				strerror(errno));
		goto failure;
	}
	pshare->count = 0;
	/* fork for proccess */
	pid = fork();
	if (-1 == pid) {
		/* error */
		fprintf(stderr, "line %d fork error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	} else if (0 < pid) {
		/* parent */
		sem_wait(&pshare->sem);
		for (i = 0; 10 > i; i++)
			fprintf(stdout, "parent pshare->count :%d\n",
					pshare->count++);
		sem_post(&pshare->sem);
	} else {
		/* child */
		sem_wait(&pshare->sem);
		for (i = 0; 10 > i; i++)
			fprintf(stdout, "child pshare->count :%d\n",
					pshare->count++);
		sem_post(&pshare->sem);
	}
finish:
	/* munmap for semphore and count */
	if (pshare != NULL && -1 == munmap(pshare, sizeof(*pshare))) {
		fprintf(stderr, "munmap error: %s\n",
				strerror(errno));
		goto failure;
	}
	pshare = NULL;
	/* close file descriptor for semaphore and count */
	if (-1 != semfd) {
		close(semfd);
		semfd = -1;
	}
	exit(ret);
failure:
	ret = EXIT_FAILURE;
	goto finish;
}
