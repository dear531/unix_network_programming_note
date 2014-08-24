#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>



int socket_func(int domain, int type, int protocol)
{
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0) {
		fprintf(stdout, "socket error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int bind_func(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if ((ret = bind(sockfd, addr, addrlen)) < 0) {
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int inet_pton_func(int af, const char *src, void *dst)
{
	int ret;
	if ((ret  = inet_pton(af, src, dst)) < 0) {
		fprintf(stdout, "family error\n");
		exit(EXIT_FAILURE);
	} else if (ret == 0) {
		fprintf(stdout, "please correct source address\n");
		exit(EXIT_FAILURE);
	}
	return ret;
}
int listen_func(int sockfd, int backlog)
{
	int ret;
	if ((ret = listen(sockfd, backlog)) < 0) {
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int accept_func(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int fd;
	if ((fd = accept(sockfd, addr, addrlen)) < 0) {
		fprintf(stdout, "accept error %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int close_func(int fd)
{
	int ret;
	if ((ret = close(fd)) < 0) {
		fprintf(stdout, "close error :%s\n", strerror(errno));
	}
	return ret;
}
int setsockopt_func(int sockfd, int level, int optname,
		     const void *optval, socklen_t optlen)
{
	int ret;
	if ((ret = setsockopt_func(sockfd, level, optname, optval, optlen)) < 0) {
		fprintf(stdout, "setsockopt error :%s\n", strerror(errno));
	}
	return ret;
}

int main(int argc, char *argv[])
{
	/* socket */
	int lifd;
	lifd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* set address re used */
	int opt = 1;
	setsockopt(lifd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	/* bind */
	struct sockaddr_in addr4;
	memset(&addr4, 0x00, sizeof(addr4));
	addr4.sin_family = AF_INET;
	addr4.sin_port = htons(8080);
	inet_pton_func(AF_INET, "127.0.0.1", &addr4.sin_addr);
	bind_func(lifd, (struct sockaddr *)&addr4, sizeof(addr4));
	/* listen */
	listen_func(lifd, 10);
	/* fork */
	pid_t pid;
	int cofd;
	int i;
	for (i = 0; i < 10; i++) {
		if ((pid = fork()) < 0) {
			/* error */
		} else if (pid == 0) {
			/* child */
			int n;
			char buf[10];
			cofd = accept(lifd, NULL, NULL);
			while ((memset(buf, 0x00, sizeof(buf)),
				(n = read(cofd, buf, sizeof(buf)))) != 0) {
				fprintf(stdout, "here %d buf:%s\n",
						i, buf);
			}
			close_func(cofd);
			close_func(lifd);
			exit(EXIT_SUCCESS);
		} else {
			/* perent */
		}
	}
	/* close connect fd */
	/* close socket fd */
	close_func(lifd);
	pause();
	
	return 0;
}
