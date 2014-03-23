#include <stdio.h>
#include <signal.h>

int flag = 0;
void sig_handler(int signo)
{
	if (signo == SIGQUIT)
		fprintf(stdout, "Quit from keyboard\n");
	else if (signo == SIGINT)
		flag = 1;
	return;
}
int main(int argc, char *argv)
{
	signal(SIGQUIT, sig_handler);
	signal(SIGINT, sig_handler);
	fprintf(stdout, "befer wait\n");
	sigset_t waitmask;
	sigfillset(&waitmask);
	sigdelset(&waitmask, SIGQUIT);
	sigdelset(&waitmask, SIGINT);
	while(flag == 0)
		sigsuspend(&waitmask);
	flag = 1;
	fprintf(stdout, "go on\n");
	return 0;
}
