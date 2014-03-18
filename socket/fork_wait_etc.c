#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "bit_check.h"

int main(void)
{
	pid_t pid, ppid;
	int ret;
	int status;
	int *p = NULL;
	
	pid = vfork();
	int i = 0;
	if (pid == 0)
	{
		//fprintf(stdout, "child process ret address:%p\n", &ret);
		//ret = 0x01010101;
		//fprintf(stdout, "child process ret address:%p\n", &ret);
		fprintf(stdout, "I am pid(getpid returned):%d\n", getpid());
		sleep(100);
		if (i == 0)
		{
		//	exit(0xf7);
			return 0xf7;
		}
		else
		{
			return 0;
		}
	}
	else if (pid > 0)
	{
		//fprintf(stdout, "prent process ret address:%p\n", &ret);
		fprintf(stdout, "my child pid(fork returned):%d\n",pid);
		//ret = wait(&status);
		//ret = waitpid(-1, NULL, 0);
		//ret = waitpid(-1, &status, 0);
		ret = waitpid(pid, &status, 0);
		//siginfo_t infop;
		//ret = waitid(P_PID, pid, &infop, WNOHANG);
		bit_check(&status, sizeof(status));
		fprintf(stdout, "my child pid(wait returned):%d, status:%#x\n", ret, status);
		ret = WIFSIGNALED(status);
		if (ret)
		{
			fprintf(stdout, "terminated by a signal");
			ret = WTERMSIG(status);
			fprintf(stdout, "number:%d\n", ret);

		}

		ret = WIFEXITED(status);
		fprintf(stdout, "child return value:%d\n", ret);
#if 0
/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define	__WTERMSIG(status)	((status) & 0x7f)

/* Nonzero if STATUS indicates normal termination.  */
#define	__WIFEXITED(status)	(__WTERMSIG(status) == 0)

# define WIFEXITED(status)	__WIFEXITED (__WAIT_INT (status))
#endif
		ret = WEXITSTATUS(status);
		fprintf(stdout, "child return status value:%#x\n", ret);
#if 0
#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)
# define WEXITSTATUS(status)	__WEXITSTATUS (__WAIT_INT (status))
#endif
	}
	else
	{
		fprintf(stdout ,"create child error:%s\n", strerror(errno));
	}
	return 0;
}
