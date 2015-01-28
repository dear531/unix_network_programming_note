#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

/* init mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* cleanup handle function */
void mutex_cleanup(void *arg)
{
	fprintf(stdout, "this is cancelup oprated\n");
	pthread_mutex_unlock((pthread_mutex_t*)arg);
	return;
}

/* pthread1 */
void *pthread1(void *arg)
{
	int ret;
	pthread_mutex_lock(&mutex);
	pthread_cleanup_push(mutex_cleanup, &mutex);
	fprintf(stdout, "pthread1 got mutex\n");
	sleep(3);
	fprintf(stdout, "cancel pthread here not appear\n");
	pthread_cleanup_pop(0);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
/* pthread2 */
void *pthread2(void *arg)
{
	sleep(1);
	pthread_cancel((pthread_t)arg);
	pthread_mutex_lock(&mutex);
	fprintf(stdout, "pthread2 got mutex\n");
	pthread_mutex_unlock(&mutex);
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
	/* destroy mutex */
	pthread_mutex_destroy(&mutex);
	return 0;
}
