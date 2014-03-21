#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <strings.h>

char globle[] = "0";

#define SAVE_MASK	0	/* flag of select either function setjmp and sigsetjmp */
#if SAVE_MASK
sigjmp_buf j;
#else
jmp_buf j;
#endif

/* signal handle function for SIGQUIT and SIGINT */
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
#if SAVE_MASK
	siglongjmp(j,1);
#else
	longjmp(j, 1);
#endif
	return;
}

/* main function */
int main(int argc, char *argv[])
{
	struct sigaction act, actint, oact;
	sigset_t set;

	/* set SIGQUIT function and mask etc */
	act.sa_handler = sig_quit;
	act.sa_flags = SA_NODEFER;
	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, SIGQUIT);
	sigaction(SIGQUIT, &act, NULL);

	/* set SIGINT function and mask etc */
	actint.sa_handler = sig_quit;
	actint.sa_flags = SA_NODEFER;
	sigfillset(&actint.sa_mask);
	sigdelset(&actint.sa_mask, SIGINT);
	sigaction(SIGINT, &actint, NULL);
#if 0
	/* set system mask */
	sigprocmask(SIG_SETMASK, NULL, &set);
	sigdelset(&set, SIGQUIT);
	sigdelset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);
#endif

#if 1
	/* setjmp and sigsetjmp loop check and test */
# if SAVE_MASK
	if (sigsetjmp(j, 1) == 0)
# else
	if (setjmp(j) == 0)
# endif
	{
		for ( ; ; )
		{
			write(STDOUT_FILENO, "m", sizeof("m"));
			sleep(1);
		}
	}
	else
	{
		for ( ; ; )
		{
			write(STDOUT_FILENO, "r", sizeof("r"));
			sleep(1);
		}
	}
#endif
	/* main loop show */
	for ( ; ; )
	{
		write(STDOUT_FILENO, ".", sizeof("."));
		sleep(1);
	}

	return 0;
}
