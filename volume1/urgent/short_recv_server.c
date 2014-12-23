#include <stdio.h>

#include "share.h"

int create_lisfd(void)
{
	int lisfd = -1;
	/* socket */
	lisfd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* set address listen reuse */
	int opt = 1;
	setsockopt_func(lisfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
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
int confd = -1;
void sig_urg(int signum)
{
	int	sav_err;
	char buf[1024];
	bzero(buf, sizeof(buf));
	recv_func(confd, buf, sizeof(buf), MSG_OOB);
	fprintf(stdout, "this is urgent data :%s\n",buf);
	sav_err	= errno;
	errno 	= sav_err;
	return;
}
int main(int argc, char *argv[])
{
	/* conbine socket and bind and listen */
	int lisfd;
	lisfd = create_lisfd();
/* choise select or siganl for operation */
#define SELECT_SIGNAL_FLAG	1
#if SELECT_SIGNAL_FLAG
	/* set nonbloc for listen file descriptor */
	int opt;
	opt = fcntl_func(lisfd, F_GETFL, 0);
	fcntl_func(lisfd, F_SETFL, opt | O_NONBLOCK);
	/* set received buffer size */
	int recvbufsize = 1024;
	setsockopt_func(lisfd, SOL_SOCKET, SO_RCVBUF, &recvbufsize, sizeof(recvbufsize));
	/* waiting for connect of client */
	fd_set accset;
	FD_ZERO(&accset);
	FD_SET(lisfd, &accset);
	select_func(lisfd + 1, &accset, NULL, NULL, NULL);
#endif
	/* accept */
	confd = accept(lisfd, NULL, NULL);
/* choise select or siganl for operation */
#if SELECT_SIGNAL_FLAG
	int en = 1;
	signal_func(SIGURG, sig_urg);
	fcntl_func(confd, F_SETOWN, getpid());
	char buf[1024];
	int i = 0;
	for ( ; ; )
	{
		bzero(buf, sizeof(buf));
		if ((recv_func(confd, buf, sizeof(buf), 0)) == 0)
		{
			fprintf(stdout, "client close \n");
			close(confd);
			confd = -1;
			break;
		}
		fprintf(stdout, "%d :normal data buf :%s\n", ++i, buf);
		sleep(1);
	}
#else
	char buf[1024];
	fd_set rset;
	fd_set xset;
	FD_ZERO(&rset);
	FD_ZERO(&xset);
	int i = 0;
	int n = 0;
	for ( ; ; )
	{
		FD_SET(confd, &rset);
		FD_SET(confd, &xset);
		select_func(confd + 1, &rset, NULL, &xset, NULL);
		if (FD_ISSET(confd, &rset))
		{
			bzero(buf, sizeof(buf));
			if ((recv_func(confd, buf, sizeof(buf), 0)) == 0)
			{
				fprintf(stdout, "client close\n");
				close(confd);
				confd = -1;
				break;
			}
			fprintf(stdout, "this select operated %d :buf: %s\n", ++i, buf);
		}
		if (FD_ISSET(confd, &xset))
		{
			bzero(buf, sizeof(buf));
			/* enforce set not in first byte of buffer */
			buf[0] = 'x';
			n = recv_func(confd, buf, sizeof(buf), MSG_OOB);
			/* not urgent data also return 1 */
			fprintf(stdout, "n:%d\n", n);
			/* show recv buf content, already buf[0] rewritten */
			fprintf(stdout, "is urgent data :%s\n", buf);
		}
		sleep(1);
	}
#endif
	close(confd);
	close(lisfd);
	
	
	return 0;
}
