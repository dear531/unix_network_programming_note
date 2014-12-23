#include <stdio.h>

#include "share.h"

int fd = -1, count = 0;

void sig_urg(int signum)
{
	int sav_err;
	sav_err	= errno;
	char buf[1024];
	bzero(buf, sizeof(buf));
	recv_func(fd, buf, 1, MSG_OOB);
	send_func(fd, "1", sizeof("1") - 1, MSG_OOB);
	count = 0;
	fprintf(stdout, "count :%d\n", count);
	errno	= sav_err;
	return;
}
void sig_alm(int signum)
{
	if (++count >= 5)
		exit(EXIT_FAILURE);
	fprintf(stdout, "count :%d\n", count);
	alarm(1);
	return;
}

int create_lisfd(void)
{
	int lisfd = -1;
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
	return lisfd;
}
int main(int argc, char *argv[])
{
	/* combine socket and bind and listen */
	int	lisfd;
	lisfd	= create_lisfd();
	/* accept */
	fd	= accept_func(lisfd, NULL, NULL);
	/* set signal function of urgent */
	signal_func(SIGURG, sig_urg);
	/* set signal function of alarm */
	signal_func(SIGALRM, sig_alm);
	/* set urgent signal owner */
	fcntl_func(fd, F_SETOWN, getpid());
	alarm(5);
	char buf[1024];
	int n = 0;
	for ( ; ; )
	{
		send_func(fd, "data", sizeof("data"), 0);
		bzero(buf, sizeof(buf));
		if ((n = recv_func(fd, buf, sizeof(buf), 0)) == 0)
		{
			fprintf(stdout, "peer already close\n");
			exit(EXIT_FAILURE);
		}
		fprintf(stdout, "n : %d recv buf :%s\n", n, buf);
		sleep(1);
	}
	/* close listen */
	close(fd);
	close(lisfd);

	return 0;
}
