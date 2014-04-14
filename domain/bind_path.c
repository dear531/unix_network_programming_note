#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <strings.h>

int main(int argc, char *argv[])
{
	int fd;
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		fprintf(stdout, "socket error :%s\n", strerror(errno));

	struct sockaddr_un dsaddr;
	bzero(&dsaddr, sizeof(dsaddr));
	dsaddr.sun_family = AF_UNIX;
	bcopy("1.foo", dsaddr.sun_path, sizeof("1.foo"));
	if (bind(fd, (struct sockaddr*)&dsaddr, sizeof(dsaddr)) < 0)
		fprintf(stdout, "bind error %s\n", strerror(errno));
	struct sockaddr_un dgaddr;
	bzero(&dgaddr, sizeof(dgaddr));
	int dlen = sizeof(dgaddr);
	fprintf(stdout, "pathname:%s, dlen :%d\n", dgaddr.sun_path, dlen);
	if (getsockname(fd, (struct sockaddr*)&dgaddr, &dlen) < 0)
		fprintf(stdout, "getsockname error :%s\n", strerror(errno));
	fprintf(stdout, "pathname:%s, dlen :%d\n", dgaddr.sun_path, dlen);
	sleep(10);
	unlink("1.foo");

		return 0;
}
