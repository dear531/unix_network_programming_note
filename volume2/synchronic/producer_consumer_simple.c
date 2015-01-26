/*
 * pthread_mutex_lock         --|
 *                              +--lock
 * entry pthread_cond_wait    --|
 *                              +--unlock and sleep
 * return pthread_cond_wait   --|
 *                              +--lock
 * pthread_mutex_unlock       --+
 *
 * pthread_mutex_lock         -------------------------+
 * pthread_cond_signal        ---- wakeup cond_wait    +--lock
 * pthread_mutex_unlock       -------------------------+
 */

#include <stdio.h>
#include <pthread.h>

#define ARRAY_NUM	100000
#define PTHREAD_NUM	20

struct increment {
	pthread_mutex_t lock;
	int record[ARRAY_NUM];
	int putn;
	int value;
} increment = {
	.putn = 0,
	.value = 0,
};
int consumer_wait(int i)
{
	pthread_mutex_lock(&increment.lock);	
	if (increment.putn < i) {
		pthread_mutex_unlock(&increment.lock);	
		return i;
	}
	if (increment.record[i] != i) {
		fprintf(stdout, "increment.record[%d]:%d != %d\n",
				i, increment.record[i], i);
	}
	if (0 == increment.record[i] % 10000) {
		fprintf(stdout, "increment.record[%d]:%d\n",
				i, increment.record[i]);
	}
	pthread_mutex_unlock(&increment.lock);	
	return i;
}
/* consumer function */
void *consumer(void *arg)
{
	int i, tmp = -1, ret;
	for (i = 0; ARRAY_NUM > i; ) {
		ret = consumer_wait(i);
		if (ret != tmp) {
			tmp = ret;
			i++;
		}
	}
	return arg;
}

/* producer function */
void *producer(void *arg)
{
	for ( ; ; ) {
		pthread_mutex_lock(&increment.lock);
		/* critical region */
		if (ARRAY_NUM < increment.value) {
			pthread_mutex_unlock(&increment.lock);
			return arg;
		}
		increment.record[increment.putn] = increment.value;
		increment.putn++;
		increment.value++;
		pthread_mutex_unlock(&increment.lock);
		(*(int *)arg)++;
		fprintf(stdout, "arg:%d, putn:%d, value:%d\n",
				*(int*)arg, increment.putn, increment.value);
	}
	return arg;
}

/* main function */
int main(int argc, char *argv[])
{
	/* init variable */
	int count[PTHREAD_NUM];
	pthread_t pthreads[PTHREAD_NUM];
	/* create_pthread for procucer */
	int i;
	for (i = 0; PTHREAD_NUM > i; i++) {
		count[i] = 0;
#if 0
       int pthread_create(pthread_t *restrict thread,
              const pthread_attr_t *restrict attr,
              void *(*start_routine)(void*), void *restrict arg);
#endif
		pthread_create(&pthreads[i], NULL, producer, &count[i]);
	}
	/* pthread_create pthread for consumer */
	pthread_t pthread_consumer;
	/* pthread_jion for pthread of producer */
	int *ret[PTHREAD_NUM];
	for (i = 0; PTHREAD_NUM > i; i++) {
		pthread_join(pthreads[i], (void**)&ret[i]);
		fprintf(stdout, "pthreads[%d]:%d, *ret[%d]:%d\n",
				i, pthreads[i], i, *ret[i]);
	}
	pthread_create(&pthread_consumer, NULL, consumer, NULL);
	/* ptread_join for pthread of consumer */
	pthread_join(pthread_consumer, NULL);
	/* exit */
	return 0;
}
