#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <setjmp.h>

sigjmp_buf j;
void sigfunc(int sinnum)
{
	int sav_err;
	sav_err = errno;
	/* not should call fprintf function in signal, only for code viwe */
	fprintf(stdout, "received alarm signal\n");
	siglongjmp(j, 1);
	errno = sav_err;
}

	int
main(int argc, char *argv[])
{
	if (signal(SIGALRM, sigfunc) == SIG_ERR)
		fprintf(stdout, "set signal error :%s\n", strerror(errno));
	int t = 10;
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		fprintf(stdout, "socket error :%s\n", strerror(errno));
	if (alarm(t) != 0)
		fprintf(stdout, "alarm time reset %d\n", t);
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	if (connect(fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
		fprintf(stdout, "connect error :%s\n", strerror(errno));
	if (sigsetjmp(j, 1) != 0)
		goto jmp_connect;
	int n;
	char buf[1024];
	if ((n = recv(fd, buf, sizeof(buf), 0)) < 0)
		fprintf(stdout, "recv error: %s\n", strerror(errno));
jmp_connect:
		return 0;
}
