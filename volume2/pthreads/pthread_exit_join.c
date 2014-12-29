#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *pthread_func(void *arg)
{
	int *i = (int *)arg;
	while (10 > *i) {
		fprintf(stdout, "*i:%d\n", (*i)++);
		usleep(500 * 1000);
	}
	pthread_exit((void*) arg);
}
int main(int argc, char *argv[])
{
	int arg;
	pthread_t thread;
	pthread_create(&thread, NULL, pthread_func, (void*)&arg);
	int *ret;
	pthread_join(thread, (void**)&ret);
	fprintf(stdout, "ret:%p, arg addr :%p\n*ret:%d, arg:%d\n",
			ret, &arg, *ret, arg);
	return 0;
}
