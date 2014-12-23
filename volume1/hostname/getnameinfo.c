#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <stdlib.h>

	int
main(int argc, char *argv[])
{
	char	hn[NI_MAXHOST];
	char	sn[NI_MAXSERV];

	struct sockaddr_in	sa;
	bzero(&sa, sizeof(sa));
	sa.sin_family	= AF_INET;
	sa.sin_port	= htons(80);
	inet_pton(AF_INET, "220.181.111.85", &sa.sin_addr);
	bzero(hn, sizeof(hn));
	bzero(sn, sizeof(sn));
	int ret;
	ret = getnameinfo((struct sockaddr*)&sa, sizeof(sa), hn, sizeof(hn),
			sn, sizeof(sn), 0);
	if (ret < 0)
	{
		fprintf(stdout, "getnameinfo error :%s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "hn :%s, sn :%s\n", hn, sn);

	return 0;
}
