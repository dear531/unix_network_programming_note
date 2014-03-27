#include <stdio.h>
#include <signal.h>

#include "pr_mask.h"

void sig_int(int signum)
{
	
	fprintf(stdout, "is signal number :%d\n", signum);
	pr_mask("sig_int");
	sleep(2);
	return;
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_int);
	while(1)
		;
	return 0;
}
