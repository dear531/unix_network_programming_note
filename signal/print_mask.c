#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "bit_check.h"

void pr_mask(void)
{
	sigset_t mask;	
	if (sigprocmask(0, NULL, &mask) < 0)
		fprintf(stdout, "get system mask failed :%s\n", strerror(errno));
	if (sigismember(&mask, SIGUSR1))
		fprintf(stdout, "contain SIGUSR1\n");
	else
		fprintf(stdout, "not contain SIGUSR1\n");
	return;
}
int add_mask(int signum)
{
	sigset_t mask;	
	if (sigemptyset(&mask) < 0)
	{
		fprintf(stdout, "set mask empty error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (sigaddset(&mask, SIGUSR1) < 0)
	{
		fprintf(stdout, "add mask empty error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
	{
		fprintf(stdout, "set system mask signal error:%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
int main(int argc, char *argv[])
{

	sigset_t mask;	
	sigset_t pendingmask;	
	add_mask(SIGUSR1);
	pr_mask();

	sigprocmask(0, NULL, &mask);
	fprintf(stdout, "system mask\n");
	bit_check(&mask, sizeof(mask));

	sigpending(&pendingmask);
	fprintf(stdout, "pending mask\n");
	bit_check(&pendingmask, sizeof(pendingmask));

	return 0;
}
