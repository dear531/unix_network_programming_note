#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


#include <string.h>
#include <errno.h>


int
main(void)
{
	int sckfd;
	struct sockaddr_in saddr;
	int ret;
	sckfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sckfd < 0)
		fprintf(stdout, "%d %s\n", __LINE__, strerror(errno));
	memset(&saddr, 0x00, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	ret = connect(sckfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0)
		fprintf(stdout, "%d %s\n", __LINE__, strerror(errno));
	fprintf(stdout, "ret:%d\n", ret);
	char buf[1024] = "1234";
	send(sckfd, buf, strlen(buf), 0);
	memset(buf, 0x00, sizeof(buf));
	recv(sckfd, buf, strlen(buf), 0);
	close(sckfd);
	return 0;
}
