/*
 * usage: gcc msg_peek.c
 * $./a.out(server)
 * $nc 127.0.0.1 8000[enter](client)
 * 1234567890(client ipput to stdin)
 * see server peek
 * stdout first msg_peek buff
 * second normal data
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	/* socket */
	int sockfd;
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* bind */
	if (0 > sockfd) {
		fprintf(stderr, "socket error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET,
	saddr.sin_port = htons(8000);
	int ret;
	ret = inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	if (0 > ret) {
		fprintf(stderr, "inet_pton error:%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	} else if (0 == ret) {
		/* inval address of family */
		fprintf(stdout, "inval address for ipv4\n");
	} else {
		/* success */
	}
	ret = bind(sockfd, (struct sockaddr*)&saddr, (socklen_t)sizeof(saddr));
	if (0 > ret) {
		fprintf(stderr, "bind error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* listen */
	listen(sockfd, 10);
	/* accept */
	int confd;
	confd = accept(sockfd, NULL, NULL);
	if (0 > confd) {
		fprintf(stderr, "accept error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* recv and send */
	char buff[1024] = {0};
	int buffsize = sizeof(buff);
	for ( ; ; ) {
		memset(buff, 0x00, buffsize);
		recv(confd, buff, buffsize, MSG_PEEK);
		fprintf(stdout, "buff:%s\n", buff);
		memset(buff, 0x00, buffsize);
		recv(confd, buff, buffsize, 0);
		fprintf(stdout, "buff:%s\n", buff);
	}
	/* close */
	if (0 < sockfd) {
		close(sockfd);
		sockfd = -1;
	}
	if (0 < confd) {
		close(confd);
		confd = -1;
	}

	return 0;
}
