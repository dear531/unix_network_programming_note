#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void sigfunc1(int sinnum)
{
	return;
}

void sigfunc2(int sinnum)
{
	return;
}

typedef void(*sighandle)(int);
int main(int argc, char *argv[])
{
	sighandle sig_f1 = NULL;
	sighandle sig_f2 = NULL;

	if ((sig_f1 = signal(SIGQUIT,sigfunc1)) == SIG_ERR)
		fprintf(stdout, "set signal error :%s\n", strerror(errno));
	/* SIG_DFL equal void(*NULL)(int) */
	fprintf(stdout, "sig_f1 :%p\n", sig_f1);
	fprintf(stdout, "SIG_DFL :%p\n", SIG_DFL);
	if ((sig_f2 = signal(SIGQUIT,sigfunc2)) == SIG_ERR)
		fprintf(stdout, "set signal error :%s\n", strerror(errno));
	/* sigfunc1 should equal sigfunc1 address */
	fprintf(stdout, "sigfunc1 :%p\n", sigfunc1);
	fprintf(stdout, "sig_f2 :%p\n", sig_f2);

	/* SIG_ERR is void (*-1)(int) */
	fprintf(stdout, "SIG_ERR :%p\n", SIG_ERR);

	return 0;
}
