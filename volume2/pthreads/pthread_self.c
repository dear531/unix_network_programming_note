#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *pthread_func(void *arg)
{
	fprintf(stdout, "pthread:%d\n", pthread_self());
	pthread_exit((void*) arg);
}
int main(int argc, char *argv[])
{
	int arg;
	pthread_t thread;
	pthread_create(&thread, NULL, pthread_func, (void*)&arg);
	int *ret;
	pthread_join(thread, (void**)&ret);
	fprintf(stdout, "child pthread:%d, pthread:%d\n",
			thread, pthread_self());
	return 0;
}
