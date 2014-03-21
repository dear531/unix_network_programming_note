#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <strings.h>

char globle[] = "0";

jmp_buf j;
void sig_quit(int signum)
{
	int i;
	fprintf(stdout, "catch signum %d\n", signum);
	char c[2];
	globle[0]++;
	bcopy(globle, c, sizeof(c));
	for (i = 0; i < 5; i++)
	{
		write(STDOUT_FILENO, &c, sizeof(c));
		sleep(1);
	}
//	longjmp(j,1);
	return;
}

int main(int argc, char *argv[])
{
	struct sigaction act, actint, oact;
	sigset_t set;
#if 0
           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };

#endif

	act.sa_handler = sig_quit;
	act.sa_flags = SA_NODEFER;
	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, SIGQUIT);
	sigaction(SIGQUIT, &act, NULL);

	actint.sa_handler = sig_quit;
	actint.sa_flags = SA_NODEFER;
	sigfillset(&actint.sa_mask);
	sigdelset(&actint.sa_mask, SIGINT);
	sigaction(SIGINT, &actint, NULL);

	sigprocmask(SIG_SETMASK, NULL, &set);
	sigdelset(&set, SIGQUIT);
	sigdelset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);

#if 0
	if (setjmp(j) == 0)
	{
		for ( ; ; )
		{
			write(STDOUT_FILENO, );
			sleep(1);
		}
	}
	else
	{
		for ( ; ; )
		{
			write();
			sleep(1);
		}
	}
#endif
	for ( ; ; )
	{
		write(STDOUT_FILENO, ".", sizeof("."));
		sleep(1);
	}

	return 0;
}
