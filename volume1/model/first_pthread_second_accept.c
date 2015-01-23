#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <stdlib.h>
#include <errno.h>

/* pthread_function */
void *pthread_connect(void *arg)
{
	int n;
	int *fd = (int *)arg;
	char buff[1024];
	pthread_t ptrdid;
	ptrdid = pthread_self();
	for ( ; memset(buff, 0x00, sizeof(buff)); ) {
		/* recv */
		n = recv(*(int *)fd, buff, sizeof(buff), 0);
		if (0 > n) {
			/* recv error */
			fprintf(stderr, "recv error :%s in %d\n", ptrdid);
			goto finish;
		} else if (0 == n) {
			/* peer close */
			fprintf(stdout, "peer close in pthread %d\n", ptrdid);
			close(*fd);
			*fd = -1;
			goto finish;
		} else {
			/* success recv */
			fprintf(stdout, " in pthread %d buff:%s\n", ptrdid, buff);
		}
		/* send */
		send(*fd, buff, strlen(buff), 0);
	}
finish:
#if 0
	/* can not detach :Segmentation fault */
	pthread_detach(ptrdid);
#endif
	pthread_exit(NULL);
}
/* main */
int main(int argc, char *argv[])
{
	/* socket */
#if 0
       int socket(int domain, int type, int protocol);
#endif
	int sockfd;
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (0 > sockfd) {
		fprintf(stderr, "socket error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	int ret;
	/* set reuse address ip */
	int opt = 1;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (0 > ret) {
		fprintf(stderr, "setsockopt error :%s\n",
				strerror(errno));
	}
	/* bind */
#if 0
	int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
         struct sockaddr_in {
             sa_family_t    sin_family; /* address family: AF_INET */
             u_int16_t      sin_port;   /* port in network byte order */
             struct in_addr sin_addr;   /* internet address */
         };
#endif
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
#if 0
   int inet_pton(int af, const char *src, void *dst);
#endif
	ret = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	if (0 > ret) {
		/* inet_pton error */
		fprintf(stderr, "inet_pton error:%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	} else if (0 == ret) {
		/* address format invalid */
		fprintf(stdout, "invalid address in family\n");
	} else {
		/* success */
	}
	ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if (0 > ret) {
		fprintf(stderr, "bind erorr :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* listen */
#define CONNECT_MAXNUM	2
	ret = listen(sockfd, CONNECT_MAXNUM);
	if (0 > ret) {
		fprintf(stderr, "listen error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	pthread_t ptrdid;
	int confd[CONNECT_MAXNUM];
	int i;
	for (i = 0; CONNECT_MAXNUM > i; i++) {
		confd[i] = -1;
	}
	for (i = 0; ; CONNECT_MAXNUM - 1 <= i ? i = 0 : i++) {
		if (0 < confd[i]) {
			continue;
		}
		/* accept */
		fprintf(stdout, "i:%d\n", i);
		confd[i] = accept(sockfd, NULL, NULL);
		if (0 > confd[i]) {
			fprintf(stderr, "accept error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
		/* create_pthread */
#if 0
		   int pthread_create(pthread_t *restrict thread,
				  const pthread_attr_t *restrict attr,
				  void *(*start_routine)(void*), void *restrict arg);
#endif
		pthread_create(&ptrdid, NULL, pthread_connect, (void*)&confd[i]);
	}
	for (i = 0; CONNECT_MAXNUM > i; i++) {
		/* close connect fd */
		if (0 < confd[i]) {
			close(confd[i]);
			confd[i] = -1;
		}
	}
	/* close listen fd */
	if (0 < sockfd) {
		close(sockfd);
		sockfd = -1;
	}
	return 0;
}
