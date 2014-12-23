#include <stdio.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	pid_t ppid;
	pid = fork();
	if (pid == 0)
	{
		ppid = getppid();
		pid = getpid();
		fprintf(stdout, "is child:ppid:%d, pid:%d\n", ppid, pid);
	}
	else if (pid > 0)
	{
		ppid = getpid();
		fprintf(stdout, "parent: child pid:%d, cureent pid:%d\n", pid, ppid);
	}
	else
	{
		fprintf(stdout, "create process fialure\n");
	}
	return 0;
}
