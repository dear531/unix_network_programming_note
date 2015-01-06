#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void print_siginfo_child(int signum, siginfo_t *info, void *context)
{
	fprintf(stdout, "signum:%d, si_code:%d, si_value:%d\n",
			signum, info->si_code, info->si_value);
	return;
}
int main(int argc, char *argv[])
{
	fprintf(stdout, "sigmax :%ld, sigmin:%ld\n",
			SIGRTMIN, SIGRTMAX);
	pid_t pid;
	union sigval value = {.sival_int = 1};
	pid = fork();
	int i, j;
	if (0 > pid) {
		/* error */
	} else if (0 < pid) {
		/* parent */
		/* sigqueue */
		sleep(3);
		for (i = 0; 3 > i; i++) {
			for (j = 0; 3 > j; j++) {
				sigqueue(pid, SIGRTMIN + j, value);
				sleep(1);
			}
			sleep(2);
		}
	} else {
		/* child */
		/* set mask */
		sigset_t newmask;
		sigemptyset(&newmask);
		struct sigaction act, oldact;
		act.sa_sigaction = print_siginfo_child;
		act.sa_flags = SA_SIGINFO;
		act.sa_mask = newmask;
		for (i = 0; 3 > i; i++) {
			sigaddset(&newmask, SIGRTMIN + i);
		}
		for (i = 0; 3 > i; i++) {
			sigaction(SIGRTMIN + i, &act, &oldact);
		}
		for ( ; ; )
			pause();
		exit(EXIT_FAILURE);
	}
	return 0;
}
