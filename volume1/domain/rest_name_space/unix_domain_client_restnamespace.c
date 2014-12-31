#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

int main(void)
{
	int sockfd;
	sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (0 > sockfd) {
		fprintf(stderr, "socket error :%s\n", strerror(errno));
		exit(EXIT_SUCCESS);
	}
	struct sockaddr_un sock_un;
	bzero(&sock_un, sizeof(sock_un));
	sock_un.sun_family = PF_UNIX;
	bcopy("\0", sock_un.sun_path, sizeof("\0"));
	int ret;
	int n;
	ret = connect(sockfd, (const struct sockaddr *)&sock_un, sizeof(sock_un));
	char buff[] = "this is client rest name space";
	if (0 > ret) {
		fprintf(stderr, "connect error :%s\n", strerror(errno));
		exit(EXIT_SUCCESS);
	}
	for ( ; ; ) {
		n = write(sockfd, buff, sizeof(buff));
		if (0 > n) {
			fprintf(stderr, "send error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	if (-1 != sockfd)
		close(sockfd);
	
	return 0;
}
