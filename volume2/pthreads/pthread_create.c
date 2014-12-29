#include <stdio.h>
#include <pthread.h>

void *pthread_func(void *arg)
{
	int *i = (int *)arg;
	while (1000 > *i) {
		fprintf(stdout, "*i:%d\n", (*i)++);
		usleep(500 * 1000);
	}
}
int main(int argc, char *argv[])
{
	int arg;
	pthread_t thread;
	pthread_create(&thread, NULL, pthread_func, (void*)&arg);
	while (1) {
		fprintf(stdout, "this is main pthread arg:%d\n",
				arg);
		sleep(2);
	}
	return 0;
}
