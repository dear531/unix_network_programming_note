#include <stdio.h>
#include <pthread.h>

struct {
	pthread_rwlock_t rwlock;
	int data;
} sync;

void *pthread_rd(void * arg)
{
	for ( ; ; ) {
		/* get rwlock for read */
		pthread_rwlock_rdlock(&sync.rwlock);
		fprintf(stdout, "sync.data:%d in read pthread %d\n",
				sync.data, (int)pthread_self());
		/* release rwlock */
		pthread_rwlock_unlock(&sync.rwlock);
		usleep(1000 * 100);
	}
	pthread_exit(NULL);
}

void *pthread_wr(void * arg)
{
	for ( ; ; ) {
		/* get rwlock for write */
		pthread_rwlock_wrlock(&sync.rwlock);
		/* write data */
		sync.data++;
		fprintf(stdout, "write pthread %d write data :%d\n",
				(int)pthread_self(), sync.data);
		/* relase rwlock */
		pthread_rwlock_unlock(&sync.rwlock);
		usleep(1000 * 100);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	/* rwlock init */
	pthread_rwlock_init(&sync.rwlock, NULL);
	/* data init */
	sync.data = 0;
	/* create thread 10 */
	int i;
	pthread_t ptrdid;
	for (i = 0; 10 > i; i++) {
		if (0 == i)
			pthread_create(&ptrdid, NULL, pthread_wr, NULL);
		else
			pthread_create(&ptrdid, NULL, pthread_rd, NULL);
		usleep(1000 * 100);
	}
	for ( ; ; )
		pause();
	/* rwlock destroy */
	pthread_rwlock_destroy(&sync.rwlock);
	return 0;
}
