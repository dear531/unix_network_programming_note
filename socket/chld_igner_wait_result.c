#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include "bit_check.h"

typedef void sigfunc(int);

sigfunc* signal(int signo, sigfunc *func)
{
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef            SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;    /* SunOS 4.x */
#endif
    } else {
#ifdef           SA_RESTART
        act.sa_flags |= SA_RESTART;    /* SVR4, 4.4BSD */
#endif
    }
    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }
    return oact.sa_handler;
}
void func_waitpid(int signo) {
    pid_t pid;
    int stat;
    //while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
	    pid = wait(&stat);
	    {
        printf("child %d exit\n", pid);
    }
    return;
}

int main(void)
{
	pid_t pid;
	int listfd, connfd;
	struct sockaddr_in sraddr;

//	signal(SIGCHLD, &func_waitpid);
	signal(SIGCHLD, SIG_IGN);

	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sraddr.sin_family = AF_INET;
	sraddr.sin_port = htons(8000);
	//inet_pton(AF_INET, "127.0.0.1", &sraddr.sin_addr);
	sraddr.sin_addr.s_addr = htonl(INADDR_ANY);
#if 1
	int ret;
	ret = bind(listfd, (struct sockaddr *)&sraddr, sizeof(sraddr));
	if (ret < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		goto eexit;
	}
#endif
	listen(listfd, 200);
	int i;
	for (i = 0; i < 200; i++)
	{
		connfd = accept(listfd, NULL, NULL);
		if (connfd < 0)
		{
			fprintf(stdout, "accpet error :%s\n", strerror(errno));
			goto eexit;
		}
		pid = fork();
		if (pid == 0)
		{
			/* is child process */
			struct sockaddr ss;
			socklen_t slen;
			slen = sizeof(ss);
			getpeername(connfd, &ss, &slen);
			char *p = NULL;
			p = inet_ntoa(((struct sockaddr_in*)&ss)->sin_addr);
			fprintf(stdout, "get ip %s, port %d\n", p, ntohs(((struct sockaddr_in*)&ss)->sin_port));
			
			fprintf(stdout, "is child process\n");

			close(connfd);
			close(listfd);
			exit(EXIT_SUCCESS);
		}
		else if (pid > 0)
		{
			/* is parent process */
			close(connfd);
		}
		else
		{
			/* error */
			fprintf(stdout, "fork process error: %s", strerror(errno));
		}
	}
	int pid_s;
	pid_s = wait(NULL);
	if (pid_s < 0)
		fprintf(stdout, "pid_s :%d\n", pid_s);
eexit:
	close(listfd);
	return 0;
}
