#include <stdio.h>
#include <pthread.h>

/* init rwlock */
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

/* pthread1 */
void *pthread1(void *arg)
{
	sleep(3);
	pthread_rwlock_wrlock(&rwlock);
	fprintf(stdout, "pthread1 got rwlock\n");
	pthread_rwlock_unlock(&rwlock);
	pthread_exit(NULL);
}
/* pthread2 */
void *pthread2(void *arg)
{
	pthread_rwlock_rdlock(&rwlock);
	fprintf(stdout, "pthread2 got rwlock\n");
	sleep(1);
	pthread_cancel((pthread_t)arg);
	sleep(1);
	pthread_rwlock_unlock(&rwlock);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	/* create pthread 1,2 */
#if 0
       int pthread_create(pthread_t *restrict thread,
              const pthread_attr_t *restrict attr,
              void *(*start_routine)(void*), void *restrict arg);
#endif
	pthread_t ptrdid1, ptrdid2;
	pthread_create(&ptrdid1, NULL, pthread1, NULL);
	pthread_create(&ptrdid2, NULL, pthread2, (void **)ptrdid1);
	/* join */
	void *status;
	pthread_join(ptrdid1, &status);
	if (PTHREAD_CANCELED != status) {
		fprintf(stdout, "pthread1 status %p\n", status);
	}
	pthread_join(ptrdid2, &status);
	if (NULL != status) {
		fprintf(stdout, "pthread2 status %p\n", status);
	}
	/* destroy rwlock */
	pthread_rwlock_destroy(&rwlock);
	return 0;
}
