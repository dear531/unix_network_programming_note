#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int ret;
	pid = getpid();
	fprintf(stdout, "before daemon the process pid :%d\n", pid);
	ret = daemon(1, 1);
	pid = getpid();
	fprintf(stdout, "first daemon the process pid :%d, ret :%d\n", pid, ret);
	ret = daemon(0, 0);
	pid = getpid();
	fprintf(stdout, "second daemon the process pid :%d, ret :%d\n", pid, ret);
	sleep(10);
	
	return 0;
}
