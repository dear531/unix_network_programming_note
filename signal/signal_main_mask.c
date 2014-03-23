#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "pr_mask.h"

void sig_quit_int(int signum)
{
	char buf[64], prbuf[2];
	int i;
	sprintf(buf, "in sig_quit_int signal %d", signum);
	pr_mask(buf);
	sprintf(prbuf, "%d", signum);
	for (i = 0; i < 10; i++)
	{
		write(STDOUT_FILENO, prbuf, 2);
		sleep(1);
	}
	return;
}
int main(int argc, char *argv[])
{
	struct sigaction act, oact;
	sigset_t set, newset;

#if 1
	sigfillset(&newset);
	sigaddset(&newset, SIGUSR1);
	sigdelset(&newset, SIGQUIT);
	sigdelset(&newset, SIGINT);
	sigprocmask(SIG_SETMASK, &newset, NULL);
#endif

	/* set signal function mask */
	act.sa_handler = sig_quit_int;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR2);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGINT, &act, NULL);
	pr_mask("main function signal set");
	for ( ; ; )
	{
		write(STDOUT_FILENO, ".", sizeof("."));
		sleep(1);
	}

	return 0;
}
