#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>

#define CLIEN_PORT	8000
#define SRVRIP		"127.0.0.1"
#define BUFSIZE		1024
int
main(int argc, char *argv[])
{
	int confd;
	struct sockaddr_in caddr;
	char buf[BUFSIZE] = "1233";
	int n;
	int ret;
	confd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (confd < 0)
		fprintf(stdout, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));

	bzero(&caddr, sizeof(caddr));
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(CLIEN_PORT);
	ret = inet_pton(AF_INET, SRVRIP, &caddr.sin_addr);
	if (ret < 0)
		fprintf(stdout, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
	else if (ret == 0)
		fprintf(stdout, "please reset suitable protocol\n");

	ret = connect(confd, (struct sockaddr *)&caddr, sizeof(caddr));
	if (ret < 0)
		fprintf(stdout, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
	fprintf(stdout, "buf:%s\n", buf);

	n = send(confd, buf, BUFSIZE, 0);
	if (n < 0)
		fprintf(stdout, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
	fprintf(stdout, "buf:%s\n", buf);

	bzero(buf, BUFSIZE);
	n = recv(confd,buf,strlen(buf),0);
	if (n < 0)
		fprintf(stdout, "%s %d %s\n", __FILE__, __LINE__, strerror(errno));
	fprintf(stdout, "n:%d\n", n);

	close(confd);

	return 0;
}
