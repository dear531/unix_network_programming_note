#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int sockfd = -1;

/* pthread_func */
void * pthread_func(void *arg)
{
	int confd = -1;
	/* pthread_mutex_lock */
	pthread_mutex_lock(&mutex);
	/* accept */
	confd = accept(sockfd, NULL, NULL);
	/* pthread_mutex_unlock */
	pthread_mutex_unlock(&mutex);
	pthread_t ptrdid;
	ptrdid = pthread_self();
	if (0 > confd) {
		fprintf(stderr, "accept error :%s in pthread %d\n",
				strerror(errno), (int)ptrdid);
		goto finish;
	}
	/* recv and send */
	char buff[1024] = {0};
	int n;
	for ( ; memset(buff, 0x00, sizeof(buff)); ) {
		n = recv(confd, buff, sizeof(buff), 0);
		if (0 > n) {
			/* recv error */
			fprintf(stderr, "recv error :%s in pthread %d\n",
					strerror(errno), (int)ptrdid);
		} else if (0 == n) {
			/* peer close */
			fprintf(stdout, "peer close in pthread %d\n",
					(int)ptrdid);
			goto finish;
		} else {
			/* is data */
			fprintf(stdout, "buff:%s pthread %d\n", buff, (int)ptrdid);
		}
		send(confd, buff, strlen(buff), 0);
	}
finish:
	if (0 < confd) {
		close(confd);
		confd = -1;
	}
	pthread_exit(NULL);
	/* ptread_exit */
}

int main(int argc, char *argv[])
{
	/* socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (0 > sockfd) {
		fprintf(stderr, "socket error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* bind */
#if 0
       int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);
           struct sockaddr_in {
               sa_family_t    sin_family; /* address family: AF_INET */
               in_port_t      sin_port;   /* port in network byte order */
               struct in_addr sin_addr;   /* internet address */
           };
#endif
	int ret;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	ret = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	if (0 > ret) {
		/* inet_pton error */
		fprintf(stderr, "inet_pton error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	} else if (0 == ret) {
		/* ip addr farmat invalid in current famiy */
		fprintf(stdout, "address invalid\n");
	} else {
		/* success */
	}
	ret = bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr));
	if (0 > ret) {
		fprintf(stderr, "bind error :%s\n",
				strerror(errno));
			exit(EXIT_FAILURE);
	}
	/* listen */
	ret = listen(sockfd, 100);
	if (0 > ret) {
		fprintf(stderr, "listen error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* pthread_create */
#if 0
       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
#endif
	pthread_t ptrdid;
	int i;
	for (i = 0; 100 > i; i++) {
		pthread_create(&ptrdid, NULL, pthread_func, NULL);
		pthread_detach(ptrdid);
	}
	for ( ; ; )
		pause();
	/* close listen */
	if (0 < sockfd) {
		close(sockfd);
		sockfd = -1;
	}
	return 0;
}
