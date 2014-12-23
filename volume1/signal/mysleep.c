#include <stdio.h>
#include <signal.h>
void sig_alarm(int signum)
{
	/* sig_alarm function body */
	return;
}

int mysleep(n)
{
	if (signal(SIGALRM, sig_alarm) == SIG_ERR)
		return (n);
	alarm(n);
	pause();
	return alarm(0);
}
int main(int argc, char *argv[])
{
	mysleep(5);
	return 0;
}
