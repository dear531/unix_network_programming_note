#include <stdio.h> 
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

void *pt_func(void *arg)
{
#if 0 /* gettid for kernel pthread */
	pid_t pid;
	pid = gettid();
	fprintf(stdout, "gettid pthread :%d\n", pid);
#endif
	pthread_t pt = 0;
	pt = pthread_self();
	fprintf(stdout, "pthread my self :%ld\n", pt);
	pthread_exit(arg);
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
	pthread_join(pt, &status);
	fprintf(stdout, "status address :%p\n", status);
	fprintf(stdout, "pt :%ld\n", (long)pt);
	pthread_t mainpt;
	mainpt = pthread_self();
	fprintf(stdout, "main my self :%ld\n", mainpt);
#if 0 /* gettid for kernel pthread */
	pid_t pid;
	pid = gettid();
	fprintf(stdout, "main gettid :%d\n", pid);
#endif
#if 0	/* copare two pthreads ids for POSIX */
	ret = pthread_equal(pt, mainpt);
	fprintf(stdout, "ret :%d\n", ret);
	ret = pthread_equal(pt, pt);
	fprintf(stdout, "ret :%d\n", ret);
#endif

	return 0;
}
