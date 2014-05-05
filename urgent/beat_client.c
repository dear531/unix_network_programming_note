#include <stdio.h>

#include "share.h"

static int fd = -1, count = 0;
void sig_alm(int singum)
{
	int sav_err;
	sav_err	= errno;
	send_func(fd, "0", sizeof("0") - 1, MSG_OOB);
	if (++count >= 5)
		exit(EXIT_FAILURE);
	errno	= sav_err;
	alarm(1);
	return;
}

void sig_urg(int signum)
{
	int sav_err;
	sav_err	= errno;
	char buf[1024];
	bzero(buf, sizeof(buf));
	recv_func(fd, buf, 1, MSG_OOB);
	fprintf(stdout, "sig_urg recv buf:%s\n", buf);
	count = 0;
	errno	= sav_err;
	return;
}
int getsockopt_func(int sockfd, int level, int optname,
		void *optval, socklen_t *optlen)
{
	int ret;
	if ((ret = getsockopt(sockfd, level, optname, optval, optlen)) < 0)
	{
		fprintf(stdout, "getsockopt error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}

int main(int argc, char *argv[])
{
	/* create socket */
	fd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* set SO_OOBINLINE */
	int opt;
	socklen_t optlen = sizeof(opt);
	getsockopt_func(fd, SOL_SOCKET, SO_OOBINLINE, &opt, &optlen);
	fprintf(stdout, "opt :%d\n", opt);
	/* connect service */
	struct sockaddr_in	saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	connect_func(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	/* set signal handle */
	alarm(5);
	signal_func(SIGURG, sig_urg);
	signal_func(SIGALRM, sig_alm);
	/* set owner */
	fcntl_func(fd, F_SETOWN, getpid());
	/* loop send and recv */
	char buf[1024];
	for ( ; ; )
	{
		bzero(buf, sizeof(buf));
		if (recv_func(fd, buf, sizeof(buf), 0) == 0)
		{
			fprintf(stdout, "service close\n");
			close(fd);
			fd = -1;
			break;
		}
		fprintf(stdout, "recv buf normal:%s\n", buf);
		sleep(1);
#define TMP_DATA_CLIENT	"from client data"
		send_func(fd, TMP_DATA_CLIENT, strlen(TMP_DATA_CLIENT), 0);
	}
	/* close socket */
	close(fd);
	
	return 0;
}
