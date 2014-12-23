#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main(int argc, char *argv[])
{
	int fd;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#if 0
           struct sockaddr_in {
               sa_family_t    sin_family; /* address family: AF_INET */
               in_port_t      sin_port;   /* port in network byte order */
               struct in_addr sin_addr;   /* internet address */
           };

           /* Internet address. */
           struct in_addr {
               uint32_t       s_addr;     /* address in network byte order */
           };
#endif
	char buf[1024];
	struct sockaddr_in saddr;
	int n;
	int ret;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	ret = connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0)
	{
		fprintf(stdout, "connect server flaide\n");
	}
	n = write(fd, "123", sizeof("123"));
	if (n < 0)
	{
		fprintf(stdout, "read failed\n");
	}
	else if (n > 0)
	{
		fprintf(stdout, "send 123 to server\n");
	}
	ret = shutdown(fd ,SHUT_WR);
	if (ret < 0)
	{
		fprintf(stdout, "connect server flaide\n");
	}
	n = read(fd, buf, sizeof(buf));
	if (n == 0)
	{
		shutdown(fd, SHUT_RD);
	}
	else if (n < 0)
	fprintf(stdout, "n = %d\n", n);

	return 0;
}
