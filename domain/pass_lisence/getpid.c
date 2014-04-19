#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	fprintf(stdout, "getuid: %d\n", getuid());
	fprintf(stdout, "geteuid: %d\n", geteuid());

	fprintf(stdout, "getgid:%d\n", getgid());
	fprintf(stdout, "getegid:%d\n", getegid());

	fprintf(stdout, "getpid:%d\n", getpid());
	fprintf(stdout, "getppid:%d\n", getppid());
	return 0;
}
