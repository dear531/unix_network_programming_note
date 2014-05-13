#include <stdio.h> 
#include <pthread.h>

void *pt_func(void *arg)
{
	pthread_exit(arg);
	sleep(100);
}

int main(int argc, char *argv[])
{
	int ret;
	pthread_t pt = 0;
	fprintf(stdout, "pt :%ld\n", (long)pt);
	int arg;
	fprintf(stdout, "after arg addess :%p\n", &arg);
	if ((ret = pthread_create(&pt, NULL, pt_func, (void *)&arg)) > 0)
	{
		fprintf(stdout, "pthread_create error :%d\n", ret);
	}
	void *status;
	pthread_join(pt, &status);
	fprintf(stdout, "status address :%p\n", status);
	sleep(1);
	pthread_join(pt, &status);
	fprintf(stdout, "status address :%p\n", status);
	fprintf(stdout, "pt :%ld\n", (long)pt);
	sleep(100);

	return 0;
}
