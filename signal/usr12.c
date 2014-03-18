#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void func1(int a)
{
	fprintf(stdout, "catch SIGUSR1\n");
	return;
}
void func2(int a)
{
	fprintf(stdout, "catch SIGUSR2\n");
	return;
}
void sin_winch(int a)
{
	fprintf(stdout, "catch SIGWINCH\n");
	exit(10);
	return;
}

int  main(int argc, char *argv[])
{
	
	if (signal(SIGUSR1, func1) == SIG_ERR) {
		fprintf(stdout, "sigusr1 error:%s\n", strerror(errno));
	} else if(signal(SIGUSR2, func2) == SIG_ERR) {
		fprintf(stdout, "sigusr2 error:%s\n", strerror(errno));
	} else if(signal(SIGWINCH, SIG_ERR) == SIG_ERR) {
		signal(SIGWINCH, sin_winch);
		fprintf(stdout, "sin_winch is SIG_ERR\n");
	}

	int i;
	fprintf(stdout, "line:%d\n", __LINE__);
#if 0
	for (i = 0; i < 4; i++)
		pause();
#endif
	int n;
	while(1) {
		n = 0;
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	sleep(1);
	fprintf(stdout, "is n:%d\n", n++);
	}

	return 0;
}
