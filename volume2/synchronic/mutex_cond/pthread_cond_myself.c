#include <stdio.h>
#include <pthread.h>

struct {
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	int num;
} sync = {
	.cond = PTHREAD_COND_INITIALIZER,
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.num = 0,
};

void * pthread_signal(void * arg)
{
	for ( ; ; ) {
		pthread_mutex_lock(&sync.mutex);
		if (0 == sync.num) {
			fprintf(stdout, "producer\n");
			sync.num++;
		}
		if (0 != sync.num)
			pthread_cond_signal(&sync.cond);
		fprintf(stdout, "producer sleep 1\n");
		pthread_mutex_unlock(&sync.mutex);
		usleep(1); /* avoid to contend */
		fprintf(stdout, "procucer release lock\n");
	}
	pthread_exit(NULL);
}

void * pthread_wait(void * arg)
{
	for ( ; ; ) {
		pthread_mutex_lock(&sync.mutex);
		while (0 == sync.num) {
			fprintf(stdout, "consume wait lock\n");
			pthread_cond_wait(&sync.cond, &sync.mutex);
			fprintf(stdout, "consume get lock\n");
		}
		fprintf(stdout, "consume...\n");
		sleep(1);
		sync.num--;
		pthread_mutex_unlock(&sync.mutex);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	/* create pthread wait */
	pthread_t pt_wait;
	pthread_create(&pt_wait, NULL, pthread_wait, NULL);
	/* create pthread signal */
	pthread_t pt_signal;
	pthread_create(&pt_signal, NULL, pthread_signal, NULL);
	/* join pthread */
	pthread_join(pt_wait, NULL);
	pthread_join(pt_signal, NULL);
	/* exit pthread */
	return 0;
}
