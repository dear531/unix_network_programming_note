#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>

void setreuseaddr(int fd)
{
	int opt = 1;
	int optlen = sizeof(opt);
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen) < 0)
	{
		fprintf(stdout, "set reuseaddr failed\n");
		exit(EXIT_FAILURE);
	}
	return;
}
int create_conn(void)
{
	/* socket */
	int listfd;
	listfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listfd < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* bind */
	setreuseaddr(listfd);
	int ret;
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	ret = bind(listfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* listen */
	ret = listen(listfd, 10);
	if (ret < 0)
	{
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* apccept */
	int connfd;
	connfd = accept(listfd, NULL, NULL);
	if (connfd < 0)
	{
		fprintf(stdout, "accept error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return connfd;
}

void setnonblocking(int fd)
{
	/* get flie discriptor option */
	int opt;
	opt = fcntl(fd, F_GETFL);
	if (opt < 0)
	{
		fprintf(stdout, "get file discriptor option error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* set file discriptor */
	opt |= O_NONBLOCK;
	int ret;
	ret = fcntl(fd, F_SETFL, opt);
	if (ret < 0)
	{
		fprintf(stdout, "set file discrptor option error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return;
}
int main(int argc, char *argv[])
{
	/* open file discriptor */
	char *pfile = "2.txt";
	int fd;
	fd = create_conn();
	setnonblocking(fd);
	if (fd < 0)
	{
		fprintf(stdout, "open file :%s occurred error: %s\n", pfile, strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* read or write check */
	int n;
	char buf[128];
	int count = 0;
again:
	//n = recv(fd, buf, sizeof(buf), 0);
	n = read(fd, buf, sizeof(buf));
	fprintf(stdout, "read buf :%s, buflen :%d, errno :%d\n", buf, n, errno);
	if (n == -1 && errno == EAGAIN)
	{
		if (errno == EAGAIN && count < 10)
		{
			sleep(1);
			count++;
			fprintf(stdout, "block count %d\n", count);
			goto again;
		}
		fprintf(stdout, "read error :%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* close file discriptor */
	close(fd);

	return 0;
}
