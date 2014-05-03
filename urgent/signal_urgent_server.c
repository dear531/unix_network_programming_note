#include <stdio.h>

#include "share.h"
int lisfd = -1, confd = -1;
void sig_urg(int signum)
{
	int save_err;
	save_err = errno;
	char buf[1024];
	bzero(buf, sizeof(buf));
	recv_func(confd, buf, sizeof(buf) - 1, MSG_OOB);
	fprintf(stdout, "recv buf :%s\n", buf);
	errno = save_err;
	return;
}
int main(void)
{
	/* socket */
	lisfd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* bind */
	struct sockaddr_in saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	bind_func(lisfd, (struct sockaddr *)&saddr, sizeof(saddr));
	/* listen */
	listen(lisfd, 10);
	/* accept */
	confd = accept_func(lisfd, NULL, NULL);
	signal_func(SIGURG, sig_urg);
	fcntl_func(confd, F_SETOWN, getpid());
	fprintf(stdout, "confd :%d\n", confd);
	char buf[1024];
	int n = -1;
	while(1)
	{
		bzero(buf, sizeof(buf));
		n = recv_func(confd, buf, sizeof(buf), 0);
		if (n == 0)
		{
			close(confd);
			confd = -1;
			fprintf(stdout, "peer close and local close fd\n");
			break;
		}
		fprintf(stdout, "buf :%s\n", buf);
		sleep(2);
	}
	/* close connect */
	if (confd != -1)
		close(confd);
	/* close listen */
	close(lisfd);
	
	return 0;
}
