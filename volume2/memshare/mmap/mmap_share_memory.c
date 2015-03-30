#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>


#define SEM_FILE	"lock.sem"
#define MMAP_FILE	"mmap.txt"

int main(int argc, char *argv[])
{
	sem_t *sem;
	pid_t pid;
	int i;
	int *count = NULL;
	int lfd = -1, mfd = -1;
	int value = -1;
	/* create file for mmap */
	mfd = open(MMAP_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (-1 == mfd) {
		fprintf(stderr, "create file for mmap error :%s\n",
				strerror(errno));
		goto failure;
	}
	write(mfd, &value, sizeof(value));
	/* mmap */
	count = mmap(NULL, sizeof(*count), PROT_READ | PROT_WRITE, MAP_SHARED, mfd, 0);
	if (MAP_FAILED == count) {
		fprintf(stderr, "mmap error :%s\n",
				strerror(errno));
		goto failure;
	}
	fprintf(stdout, "count :%d\n",
			*(count));
	count[0] = 0;
	/* create lock file for semaphore */
	lfd = open(SEM_FILE, O_CREAT, S_IRUSR | S_IWUSR);
	if (-1 == lfd) {
		fprintf(stderr, "create file for locked semaphore file :%s\n",
				strerror(errno));
		goto failure;
	}
	if (-1 != lfd)
		close(lfd);
	/* sem_open */
	sem = sem_open(SEM_FILE, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (SEM_FAILED == sem) {
		fprintf(stderr, "sem_open error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* setbuf for stdout */
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
		for (i = 0; 10 > i; i++)
			fprintf(stdout, "parent count :%d\n",
					count[0]++);
		msync(count, sizeof(*count), MS_SYNC | MS_INVALIDATE);
		sem_post(sem);
	} else {
		/* child */
		sem_wait(sem);
		for (i = 0; 10 > i; i++)
			fprintf(stdout, "child count :%d\n",
					count[0]++);
		sem_post(sem);
		exit(EXIT_SUCCESS);
	}
	if (-1 != lfd)
		close(lfd);
	if (-1 != mfd)
		close(mfd);
	/* sem_unlink */
	sem_unlink(SEM_FILE);
	unlink(SEM_FILE);
	/* munmap */
	munmap(count, sizeof(*count));
	/* unlink file for mmap */
	unlink(MMAP_FILE);
	return 0;
failure:
	if (-1 != lfd)
		close(lfd);
	if (-1 != mfd)
		close(mfd);
	sem_unlink(SEM_FILE);
	unlink(SEM_FILE);
	/* munmap */
	munmap(count, sizeof(*count));
	/* unlink file for mmap */
	unlink(MMAP_FILE);
	exit(EXIT_FAILURE);
}
