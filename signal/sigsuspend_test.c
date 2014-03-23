#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "pr_mask.h"
void sig_quit(int signum)
{
	pr_mask("sig_quit");
	int i;
	for (i = 0; i < 10; i++)
	{
		write(STDOUT_FILENO, "+", sizeof("+"));
		sleep(1);
	}
	return;
}
int main(int argc, char *argv[])
{
	sigset_t waitset;
	sigset_t sysset;
	struct sigaction act;
	act.sa_handler = sig_quit;
	act.sa_flags = SA_NODEFER;
	sigemptyset(&act.sa_mask);

	sigaction(SIGQUIT, &act, NULL);

	sigfillset(&sysset);
	sigdelset(&sysset, SIGQUIT);
	sigdelset(&sysset, SIGINT);

	sigprocmask(SIG_SETMASK, &sysset, NULL);
	pr_mask("befer suspend");
	int i;
	for (i = 0; i < 10; i++)
	{
		write(STDOUT_FILENO, "-", sizeof("-"));
		sleep(1);
	}
#if 1
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGINT);
	sigsuspend(&waitset);
#endif
	for ( ; ; )
	{
		write(STDOUT_FILENO, ".", sizeof("."));
		sleep(1);
	}
	pr_mask("after suspend");
	return 0;
}
