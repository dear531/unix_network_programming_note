#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

void sig_handle(int signum)
{
	fprintf(stdout, "Enter signal errno :%d\n", errno);
#define SEV_ERR_VA	1	/* seva errno value flag :0 not seva, 1 seva */
#if SEV_ERR_VA
	int save_errno;
	save_errno = errno;
	fprintf(stdout, "seva errnor :%d\n", errno);
#endif
	int ret;
	struct in_addr ssa;
	/* write does not file descriptor, in this case is set errno */
	write(-1, ".", sizeof("."));
	fprintf(stdout, "here signal set current errno :%d\n", errno);
#if SEV_ERR_VA
	fprintf(stdout, "restore provios errno :%d\n", save_errno);
	errno = save_errno;
#endif
	fprintf(stdout, "leave signal errno :%d\n", errno);
	return;
}

int main(int argc, char *argv[])
{
	if (signal(SIGQUIT, sig_handle) == SIG_ERR)
	{
		fprintf(stdout, "set signal function error :%d\n", errno);
		exit(EXIT_FAILURE);
	}
	struct in_addr ssa;
	int ret;
	char buf[10];
	/* set does not exist address family give in inet_pton function */
	ret = inet_pton(100, "127.0.0.1", &ssa);
	fprintf(stdout, "call inet_pton is set should errno:%d\n", errno);
#if SEV_ERR_VA
	fprintf(stdout, "this is seva errno show\n");
#else
	fprintf(stdout, "this is not seva errno show\n");
#endif
	/* system recall function read, when signal return */
	fprintf(stdout, "***control prcess, please first key \"^+\\\" generate SIGQUIT, second input randemly to stdin\n");
	read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stdout, "***control prcess, from stdin read return buf:%s", buf);
	
	fprintf(stdout, "show errno string :%d\n", errno);

	return 0;
}
