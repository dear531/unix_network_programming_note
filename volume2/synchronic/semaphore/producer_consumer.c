#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define NBUFF		64
char *sem_file_name[] = {"nempty", "nstore", "mutex", };
#define SEM_NUM	(sizeof(sem_file_name) / sizeof(*sem_file_name))
enum sem_index {
	NEMPTY = 0,
	NSTORE,
	MUTEX,
};
struct {
	char buff[NBUFF];
	sem_t *sem[SEM_NUM];
} shared;

int sem_wait_func(sem_t *sem)
{
	int ret;
	if (-1 == (ret = sem_wait(sem))) {
		fprintf(stderr, "sem_wait error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int sem_post_func(sem_t *sem)
{
	int ret;
	if (-1 == (ret = sem_post(sem))) {
		fprintf(stderr, "sem_post error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
void *producer_pthread(void *arg)
{
	int i, count;
	for ( ; ; ) {
		sem_wait_func(shared.sem[MUTEX]);
		sem_wait_func(shared.sem[NSTORE]);
		for (i = 0, count = 0; SEM_NUM > i; i++) {
			if (-1 == shared.buff[i]) {
				shared.buff[i] = i;
				fprintf(stdout, "producer :%d\n", i);
				count++;
				if (3 <= count)
					break;
			}
		}
		sem_post_func(shared.sem[NEMPTY]);
		sem_post_func(shared.sem[MUTEX]);
		usleep(100 * 1000);
	}
	return NULL;
}
void *consumer_pthread(void *arg)
{
	int i;
	for ( ; ; ) {
		sem_wait_func(shared.sem[MUTEX]);
		sem_wait_func(shared.sem[NEMPTY]);
		for (i = 0; SEM_NUM > i; i++) {
			if (i == shared.buff[i]) {
				fprintf(stdout, "consumer :%d\n", i);
				shared.buff[i] = -1;
			}
		}
		sem_post_func(shared.sem[NSTORE]);
		sem_post_func(shared.sem[MUTEX]);
		usleep(100 * 1000);
	}
	return NULL;
}
void sig_int(int signum)
{
	/* semaphore unlink */
	int i, ret;
	for (i = 0; SEM_NUM > i; i++) {
		ret = sem_unlink(sem_file_name[i]);
		if (-1 == ret) {
			fprintf(stderr, "sem_unlink error :%s\n",
					strerror(errno));
		}
	}
	exit(EXIT_SUCCESS);
}
int main(int argc, char *argv[])
{
	if (SIG_ERR == signal(SIGINT, sig_int)) {
		fprintf(stderr, "signal error :%s\n",
				strerror(errno));
	}
	/* semaphore create */
#define FILE_MODE	(S_IRUSR | S_IWUSR)
#define FLAGS		(O_CREAT | O_EXCL | O_RDWR)
	int value[] = {0, NBUFF, 1};
	int i;
	for (i = 0; SEM_NUM > i; i++) {
		shared.sem[i] = sem_open(sem_file_name[i], FLAGS, FILE_MODE, value[i]);
		if (SEM_FAILED == shared.sem[i]) {
			fprintf(stderr, "sem_open error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	/* init buff */
	int sval;
	sem_wait_func(shared.sem[MUTEX]);
	sem_getvalue(shared.sem[MUTEX], &sval);
	fprintf(stdout, "hold mutex %d\n",
			sval);
	for (i = 0; sizeof(shared.buff) / sizeof(*shared.buff) > i; i++) {
		shared.buff[i] = -1;
	}
	sem_post_func(shared.sem[MUTEX]);
	sem_getvalue(shared.sem[MUTEX], &sval);
	fprintf(stdout, "release mutex %d\n",
			sval);
	/* create pthread */
#if 0
       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
#endif
	int ret;
	pthread_t ptrd[2];
	void *(*pthread_func[2])(void *) = {
		producer_pthread,
		consumer_pthread,
	};
	for (i = 0; sizeof(ptrd) / sizeof(*ptrd) > i; i++) {
		ret = pthread_create(ptrd + i, NULL, pthread_func[i], NULL);
		if (0 != ret) {
			fprintf(stderr, "pthread_create %d error :%s\n",
					i, strerror(errno));
			exit(EXIT_FAILURE);
		}
		usleep(100 * 1000);
	}
	/* pthread join */
	for (i = 0; sizeof(ptrd) / sizeof(*ptrd) > i; i++) {
		ret = pthread_join(ptrd[i], NULL);
		if (0 != ret) {
			fprintf(stderr, "pthread_join %d error :%s\n",
					i, strerror(errno));
		}
	}
	/* semaphore close */
	for (i = 0; SEM_NUM > i; i++) {
		ret = sem_close(shared.sem[i]);
		if (-1 == ret) {
			fprintf(stderr, "sem_close error\n");
		}
	}
	/* semaphore unlink */
	for (i = 0; SEM_NUM > i; i++) {
		ret = sem_unlink(sem_file_name[i]);
		if (-1 == ret) {
			fprintf(stderr, "sem_unlink error :%s\n",
					strerror(errno));
		}
	}

	return 0;
}
