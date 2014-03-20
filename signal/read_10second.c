#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <setjmp.h>
#include <stdlib.h>

jmp_buf evt_alarm;
void sig_alarm(int signum)
{
//	exit(EXIT_FAILURE);
	longjmp(evt_alarm, 1);
	return;
}

int main(int argc, char *argv[])
{
	int n = 0;
	char buf[10] = "12345\n";
	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
		fprintf(stdout, "set sigalarm error:%s\n", strerror(errno));
#if 1
	if (setjmp(evt_alarm) != 0)
	{
		fprintf(stdout, "read timeout\n");
		exit(EXIT_FAILURE);
	}
#endif
	fprintf(stdout, "please at 10 second input\n");
	alarm(10);
	if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
		fprintf(stdout, "read from stdin error:%s\n", strerror(errno));
	alarm(0);
	write(STDOUT_FILENO, buf, n);

	return 0;
}
