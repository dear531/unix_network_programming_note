#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_quit(int signum)
{
#ifdef SA_INTERRUPT
	fprintf(stdout, "exist interrupt macro\n");
#else
	fprintf(stdout, "not exist interrupt macro\n");
#endif
	fprintf(stdout, "catch signal num %d\n", signum);
	return;
}
typedef void (*sighandler_t)(int);
sighandler_t signal_restart(int signum, sighandler_t func)
{
	struct sigaction act, oact;
#ifdef SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;
#endif
	act.sa_handler = func;
	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, signum);
	sigprocmask(SIG_SETMASK, &act.sa_mask, &oact.sa_mask);

	if (sigaction(signum, &act, &oact) < 0)
		return (SIG_ERR);
	if (oact.sa_flags & SA_SIGINFO == SA_SIGINFO)
		return (sighandler_t)oact.sa_sigaction;
	else
		return oact.sa_handler;
}
int main(int argc, char *argv[])
{
	if (signal_restart(SIGQUIT, sig_quit) == SIG_ERR)
		fprintf(stdout, "assign failed\n");
	sleep(10);
	for ( ; ; )
	{
		write(STDOUT_FILENO, ".", sizeof("."));
		sleep(1);
	}
	return 0;
}
