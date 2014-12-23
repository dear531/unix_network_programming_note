#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>

sigjmp_buf j;
void sig_abort(int signo)
{
	siglongjmp(j, 1);
	return;
}
int main(int argc, char *argv[])
{
	int i;
	signal(SIGABRT, sig_abort);
	if (sigsetjmp(j, 1) == 0)
	{
		for (i = 0; i < 10; i++)
		{
			write(STDOUT_FILENO, "-", sizeof("-"));
			sleep(1);
		}
		abort();
	}
	else
	{
		for (i = 0; i < 10; i++)
		{
			write(STDOUT_FILENO, "+", sizeof("+"));
			sleep(1);
		}
	}
	return 0;
}
