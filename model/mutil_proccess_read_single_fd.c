#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>


int socket_func(int domain, int type, int protocol)
{
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0) {
		fprintf(stdout, "socket error :%s\n", strerror(errno));
	}
	return fd;
}
int close_func(int fd)
{
	int ret;
	if (0 != fd) {
		ret = close(fd);
		fd = -1;
	}
	return ret;
}

int main(int argc, char *argv[])
{
	int fd, confd;
	fd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in sockaddr;	
	memset(&sockaddr, 0x00, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(8080);
	inet_pton(AF_INET, "127.0.0.1", &sockaddr.sin_addr);
	if (bind(fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) {
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	listen(fd, 10);
	confd = accept(fd, NULL, NULL);
	if (confd < 0) {
		fprintf(stdout, "accept error :%s\n", strerror(errno));
	}

	pid_t pid;
	char buf[100] = {0};
	int i;
	for (i = 0; i < 10; i++) {
		if ((pid = fork()) < 0) {
			/** error **/
		} else if (pid == 0) {
			/** child **/
			close_func(fd);
			while (1) {
				sleep(10);
				memset(buf, 0x00, sizeof(buf));
				read(confd, buf, sizeof(buf));
				fprintf(stdout, "here process %d, buf:%s\n",
						i, buf);
			}
			close_func(confd);
			exit(EXIT_SUCCESS);
		} else {
			/** perent **/
			sleep(2);
		}
	}
	pause();
	close_func(confd);
	close_func(fd);

	return 0;
}



