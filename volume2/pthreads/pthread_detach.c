#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

void *thread_func(void *arg)
{
	int ret;
	fprintf(stdout, "here child pthread\n");
	sleep(2);
	ret = pthread_detach(pthread_self());
	if (0 > ret) {
		fprintf(stdout, "detach child pthread failed\n");
		exit(EXIT_FAILURE);
	}
	sleep(2);
	fprintf(stdout, "child pthread finish\n");
	pthread_exit(arg);
}

int main(int argc, char *argv[])
{
	int arg, *ret, result;
	pthread_t thread;
	pthread_create(&thread, NULL, thread_func, (void *)&arg);
	sleep(1);
#if 1
	result = pthread_join(thread, (void **)&ret);
	if (0 > result) {
		fprintf(stdout, "pthread_join error:%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
#endif
	fprintf(stdout, "main pthread finish\n");
	
	/*
	 * main finish effect child pthread finish
	 * including detach pthread finish
	 */
	return 0;
}
