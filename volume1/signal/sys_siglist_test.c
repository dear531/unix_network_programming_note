#include <stdio.h>
#include <signal.h>
#include <string.h>


int
main(int argc, char *argv[])
{
	int i;
	for (i = 0; i < SIGRTMAX; i++)
	fprintf(stdout, "sys_siglist[%d]:%s\n", i, sys_siglist[i]);
	psignal(SIGQUIT, "msg");
	fprintf(stdout, "%s\n", strsignal(SIGQUIT));
}
