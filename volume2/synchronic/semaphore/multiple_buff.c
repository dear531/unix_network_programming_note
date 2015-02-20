#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int fd;
enum sem_num {
	MUTEX = 0,
	EMPTY,
	STORE,
	SEM_MAX,
};
#define NBUFF	8
int sem_value[SEM_MAX] = {1, 0, NBUFF};
struct {
	struct {
		char data[BUFSIZ / 100];
		int n;
	} buff[NBUFF];
	sem_t sem[SEM_MAX];
} shared;
void *producer(void *arg)
{
	int i;
	for (i = 0; ; ){
		sem_wait(shared.sem + STORE);
		sem_wait(shared.sem + MUTEX);
		sem_post(shared.sem + MUTEX);
		if (0 == (shared.buff[i].n = read(fd, shared.buff[i].data, sizeof(shared.buff[i].data)))) {
			fprintf(stdout, "file finish\n");
			sem_post(shared.sem + EMPTY);
			pthread_exit(NULL);
		}
		fprintf(stdout, "producer read file to buff :%s\n",
						shared.buff[i].data);
		sleep(1);
		if (NBUFF <= ++i) {
			i = 0;
		}
		sem_post(shared.sem + EMPTY);
	}
	pthread_exit(NULL);
}

void *consumer(void *arg)
{
	int i;
	for (i = 0; ; ) {
		sem_wait(shared.sem + EMPTY);
		sem_wait(shared.sem + MUTEX);
		sem_post(shared.sem + MUTEX);
		/* oprated data */
		if (0 == shared.buff[i].n) {
			sem_post(shared.sem + STORE);
			pthread_exit(NULL);
		}
		fprintf(stdout, "consumer :\n");
		write(STDOUT_FILENO, shared.buff[i].data,
				shared.buff[i].n);
		sleep(1);
		memset(shared.buff[i].data, 0x00,
				sizeof(shared.buff[i].data));
		shared.buff[i].n = -1;
		if (NBUFF <= (++i)) {
			i = 0;
		}
		sem_post(shared.sem + STORE);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	/* check argmant command-line */
	if (2 != argc) {
		fprintf(stderr, "usage : a.out filename\n");
		exit(EXIT_FAILURE);
	}
	int i;
	/* init semaphore */
	int sval;
	for (i = 0; SEM_MAX > i;i++) {
		if (0 != sem_init(shared.sem + i, 0, sem_value[(enum sem_num)i])) {
			fprintf(stderr, "sem_init error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
		sem_getvalue(shared.sem + i, &sval);
	}
	/* init data */
	for (i = 0; NBUFF > i; i++) {
		shared.buff[i].n = -1;
	}
	/* open file */
#if 0
	int open(const char *pathname, int flags);
#endif
	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (0 > fd) {
		fprintf(stderr, "open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	char c;
	for (i = 0, c = '0'; 1000 > i; i++, c++) {
		if (0 > write(fd, &c, sizeof(c))) {
			fprintf(stderr, "write error for 1.txt\n");
			exit(EXIT_FAILURE);
		}
	}
	lseek(fd, 0, SEEK_SET);
	/* create pthread consumer and producer */
	pthread_t ptid[2];
	void *(*pthread_func[2])(void *) = {
		consumer,
		producer,
	};
#if 0
       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
#endif
	int len = sizeof(ptid) / sizeof(*ptid);
	for (i = 0; len > i; i++) {
		if (0 != pthread_create(ptid + i, NULL, pthread_func[i], NULL)) {
			fprintf(stderr, "create pthread error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	/* join pthread of consumer and producer */
	for (i = 0; len > i; i++) {
		if (0 != pthread_join(ptid[i], NULL)) {
			fprintf(stderr, "pthread_join error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	/* close file */
	if (0 < fd) {
		close(fd);
		fd = -1;
	}
	/* destroy semaphore */
	for (i = 0; SEM_MAX > i;i++) {
		if (0 != sem_destroy(shared.sem + i)) {
			fprintf(stderr, "sem_destroy error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	unlink(argv[1]);
	return 0;
}
