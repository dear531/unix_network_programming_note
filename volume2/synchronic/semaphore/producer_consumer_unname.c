#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>

/* declared sem */
enum sem_num {
	MUTEX = 0,
	EMPTY,
	STORE,
	SEM_MAX,
};
#define NBUFF	64
struct {
	sem_t sem[SEM_MAX];
	int buff[NBUFF];
	int conput, proput;
} shared;
#if 0
int sem_wait(sem_t *sem);
#endif
int sem_wait_func(sem_t *sem)
{
	int ret;
	if (0 != (ret = sem_wait(sem))) {
		fprintf(stderr, "sem_wait error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
#if 0
int sem_post(sem_t *sem);
#endif
int sem_post_func(sem_t *sem)
{
	int ret;
	if (0 != (ret = sem_post(sem))) {
		fprintf(stderr, "sem_post error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
void *producer(void *arg)
{ 
	int i;
	for ( ; ; ) {
		/* sem_wait for store */
		sem_wait_func(&shared.sem[STORE]);
		/* sem_wait for mutex */
		sem_wait_func(&shared.sem[MUTEX]);
		/* produced */
		if (NBUFF <= shared.proput) {
			/* sem_post for mutex */
			sem_post_func(&shared.sem[MUTEX]);
			/* sem_post for empty */
			sem_post_func(&shared.sem[EMPTY]);
			pthread_exit(NULL);
		} else if (-1 == shared.buff[shared.proput]) {
			shared.buff[shared.proput] =
				shared.proput;
			shared.proput++;
			fprintf(stdout, "ptid %ld produced %d\n",
					(long)pthread_self(), shared.proput);
		} else {
			fprintf(stderr, "perfrom error\n");
		}
		/* sem_post for mutex */
		sem_post_func(&shared.sem[MUTEX]);
		/* sem_post for empty */
		sem_post_func(&shared.sem[EMPTY]);
		usleep(100 * 1000);
	}
	pthread_exit(NULL);
}
void *consumer(void *arg)
{
	int i;
	for ( ; ; ) {
		/* sem_wait for store */
		sem_wait_func(&shared.sem[EMPTY]);
		/* sem_wait for mutex */
		sem_wait_func(&shared.sem[MUTEX]);
		/* consumer */
		if (NBUFF <= shared.conput) {
			/* sem_post for mutex */
			sem_post_func(&shared.sem[MUTEX]);
			/* sem_post for empty */
			sem_post_func(&shared.sem[STORE]);
			pthread_exit(NULL);
		} else if (-1 == shared.buff[shared.conput]) {
			fprintf(stdout, "not produced\n");
		} else if (shared.conput == shared.buff[shared.conput]) {
			shared.buff[shared.conput] = -1;
			shared.conput++;
			fprintf(stdout, "ptid %ld consumer %d\n",
					(long)pthread_self(), shared.conput);
		}
		/* sem_post for mutex */
		sem_post_func(&shared.sem[MUTEX]);
		/* sem_post for empty */
		sem_post_func(&shared.sem[STORE]);
		usleep(100 * 1000);
	}
	pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
	/* semaphore initialized */
	int i;
	int value[SEM_MAX] = {1, 0, NBUFF};
	for (i = 0; SEM_MAX > i; i++) {
		if (-1 == sem_init(&shared.sem[i], 0, value[i])) {
			fprintf(stderr, "sem_init error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	/* init buff */
	if (-1 == sem_wait(&shared.sem[MUTEX])) {
		fprintf(stderr, "sem_wait error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	for (i = 0; NBUFF > i; i++) {
		shared.buff[i] = -1;
	}
	shared.conput = 0;
	shared.proput = 0;
	if (-1 == sem_post(&shared.sem[MUTEX])) {
		fprintf(stderr, "sem_post error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* pthread creat */
	void *(*pthread_func[])(void *) = {
		consumer,
		consumer,
		consumer,
		producer,
		producer,
		producer,
		producer,
	};
	fprintf(stdout, "pnum :%ld\n",
			(long)sizeof(pthread_func) / sizeof(*pthread_func));
	pthread_t ptrd[sizeof(pthread_func) / sizeof(*pthread_func)];
	int ret;
	int ptrd_num = sizeof(ptrd) / sizeof(*ptrd);
	for (i = 0; ptrd_num > i; i++) {
		if (0 != (ret = pthread_create(ptrd + i, NULL,
					pthread_func[i],
					NULL))) {
			fprintf(stderr, "pthread_create error :%s\n",
					strerror(ret));
			exit(EXIT_FAILURE);
		}
		usleep(100 * 1000);
	}
	/* pthread join */
	for (i = 0; ptrd_num > i; i++) {
		if (0 != (ret = pthread_join(ptrd[i], NULL))) {
			fprintf(stderr, "pthread_join error :%s\n",
					strerror(ret));
			exit(EXIT_FAILURE);
		}
	}
	/* semaphore destroyed */
	for (i = 0; SEM_MAX > i; i++) {
		if (0 != sem_destroy(&shared.sem[i])) {
			fprintf(stderr, "sem_destroy error :%s\n",
					strerror(errno));
		}
	}
	return 0;
}
