#include <stdio.h>
#include <signal.h>
#include "bit_check.h"

int main(int argc, char *argv[])
{
	int ret;
	sigset_t set;
	sigemptyset(&set);	
	bit_check(&set, sizeof(sigset_t));

	sigaddset(&set, SIGUSR1);
	fprintf(stdout, "I'm SIGUSR1 add set\n");
	bit_check(&set, sizeof(sigset_t));

	sigfillset(&set);
	bit_check(&set, sizeof(sigset_t));

	ret = sigismember(&set, SIGUSR1);
	fprintf(stdout, "just contain SIGUSR1, ret:%d\n", ret);

	sigdelset(&set, SIGUSR1);
	fprintf(stdout, "I'm SIGUSR1 del set\n");
	bit_check(&set, sizeof(sigset_t));

	ret = sigismember(&set, SIGUSR1);
	fprintf(stdout, "just not contain SIGUSR1, ret:%d\n", ret);

	sigprocmask(0, NULL, &set);
	fprintf(stdout, "I'm process mask value\n");
	bit_check(&set, sizeof(sigset_t));

	fprintf(stdout, "sizeof(set):%ld\n", sizeof(sigset_t));
	return 0;
}
