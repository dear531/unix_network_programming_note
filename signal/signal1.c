#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void sig_usr1(int a)
{
	fprintf(stdout, "is sig_usr1, %d\n", a);
	return;
}
void sig_usr2(int a)
{
	fprintf(stdout, "is sig_usr2\n");
	return;
}

int main(int argc, char *argv[])
{
#if 0
	 sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
#endif
	sigset_t set, oldset;
	sigprocmask(SIG_SETMASK, NULL, &oldset);
//	if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
	if (signal(SIGUSR1, SIGCHLD) == SIG_ERR)
		fprintf(stdout, "sigusr1:%d, set sigusr1 error :%s\n",SIGUSR1, strerror(errno));
	else if (signal(SIGUSR2, sig_usr2) == SIG_ERR)
		fprintf(stdout, "set sigusr2 error :%s\n", strerror(errno));
		fprintf(stdout, "sigusr1:%d\n",SIGUSR1);
	for ( ; ; )
		pause();
	return 0;
}
