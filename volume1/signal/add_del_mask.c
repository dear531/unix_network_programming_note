/*
 * sigpending function usage test
 * if set mask true and signal delivered, then call sigpending() be to return 1.
 * if set mask trre and signal not delivered, then call sigpending() be to return 0.
 * use sigismember() check the signal from mask.
 */
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/* error function */
void sys_err(char * prompt)
{
	fprintf(stdout, "%s:%s", prompt, strerror(errno));
	exit(EXIT_FAILURE);
}
/* signal quit funciton */
void sig_quit(int sinnum)
{
	/* sigquit function */
	fprintf(stdout, "catch SIGQUIT\n");
	int i;
	for (i = 0; i < 10; i++)
	{
		fprintf(stdout, "sleep(%d)\n", i);
		sleep(1);
	}
#if 0
	/* restore system default  */
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		sys_err("set signal sinquit to SIN_DFL\n");
#endif
	return;
}

int main(int argc, char *argv[])
{
	int ret;
	sigset_t init, set, oldset, pendingset;
#if 1
	/* set function for signal quit */
	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
		sys_err("set sig_quit function failed");
#endif
#if 1
	/* set system mask */
	if (sigfillset(&init) < 0)
		sys_err("init fill mask error");
	/* set unblock */
	if (sigprocmask(SIG_SETMASK, &init, NULL) < 0)
		sys_err("init system mask error");
	fprintf(stdout, "block signal all\n");
#else
	/* add signal quit in mask */
	if (sigemptyset(&set))
		sys_err("empty mask error");
	if (sigaddset(&set, SIGQUIT) < 0)
		sys_err("add mask error");
	if (sigprocmask(SIG_BLOCK, &set, &oldset) < 0)
		sys_err("add signal and save old mask error");
	fprintf(stdout, "block signal SIGQUIT\n");
#endif

	/* should need signal kill */
	sleep(5);

	/* get pending signal set */
	if (sigpending(&pendingset) < 0)
		sys_err("get pending mask");

	if ((ret = sigismember(&pendingset, SIGQUIT)) == 1)
	{
		/* contain signal */
		fprintf(stdout, "contain signal\n");
#if 1
		/* clear signal quit of mask */
		if (sigemptyset(&set))
			sys_err("empty mask error\n");
		if (sigaddset(&set, SIGQUIT))
			sys_err("add mask error\n");
		if (sigprocmask(SIG_UNBLOCK, &set, NULL))
			sys_err("set mask error\n");
#else
		/* restore old system mask set */
		if (sigprocmask(SIG_SETMASK, &oldset, NULL) < 0)
			sys_err("set old mask error");
#endif
		fprintf(stdout, "unblock signal SIGQUIT\n");
	}
	else if (ret == 0)
	{
		/* not contain siganl */
		fprintf(stdout, "not contain\n");
	}
	else
	{
		/* error */
		fprintf(stdout, "get pending error\n");
	}
	/* if by signal interrupt, then sleep be quit return the call process */
	sleep(5);
	/* every need signal interrupt kill sleep */
	sleep(5);

	return 0;
}
