#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

void sys_err(char * prompt)
{
	fprintf(stdout, "%s :%s\n", prompt, strerror(errno));
	return;
}
void sig_func(int signum)
{
	int i;
	fprintf(stdout, "here sig_fucn\n");
	for (i = 0; i < 10; i++)
	{
		fprintf(stdout, "sleep(%d)\n", i);
		sleep(1);
	}
	return;
}
int main(int argc, char *argv[])
{
#if 0
	sigset_t set;
	if (sigfillset(&set))
		sys_err("set all system mask\n");
	if (sigprocmask(0, &set, NULL) < 0)
		sys_err("set system mask\n");
#endif

	struct sigaction act, oact;
	act.sa_handler = sig_func;
	act.sa_flags = SA_RESETHAND | SA_NODEFER;
	if (sigemptyset(&act.sa_mask) <0)
		sys_err("empty mask error");
#if 1
	if (sigaddset(&act.sa_mask, SIGQUIT))
		sys_err("add SIGQUIT error");
#endif
	fprintf(stdout, "here first sleep(5), waiting SIGQUIT\n");
	sleep(5);
	
	if (sigaction(SIGQUIT, &act, &oact) < 0)
		sys_err("sigaction error");
	else
		fprintf(stdout, "on mask SIGQUIT\n");
	fprintf(stdout, "here sencond sleep(5), waiting SIGQUIT\n");
	sleep(5);
	sleep(5);

	return 0;
}

