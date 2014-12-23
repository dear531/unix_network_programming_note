#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>

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
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	char buf[128];
	int count = 0;
	sleep(50);
	while(1)
	{
		count++;
		bzero(buf, sizeof(buf));
		recv(fd, buf, sizeof(buf), 0);
#if 0
		if (count % 1000000 == 0)
			fprintf(stdout, "count:%d\n", count);
#endif
		//fprintf(stdout, "receive server buf:%s\n", buf);
		//usleep(100000);
	}
	return 0;
}
