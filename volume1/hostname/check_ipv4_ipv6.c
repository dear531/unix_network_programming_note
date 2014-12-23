#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct in_addr dip4;
	struct in6_addr dip6;
	int flag = 0;
	if (argc != 2)
	{
		fprintf(stdout, "usage: a.out <ipv4(desimal dot) | ipv6 (hex colon)>\n");
		exit(EXIT_FAILURE);
	}
	if (inet_pton(AF_INET, argv[1], &dip4) == 0)
	{
		flag++;
	}
	else
	{
		fprintf(stdout, "%s is ipv4 format\n", argv[1]);
		goto is_ip4;
	}
	if (inet_pton(AF_INET6, argv[1], &dip6) == 0)
	{
		flag++;
	}
	else
	{
		fprintf(stdout, "%s is ipv6 format\n", argv[1]);
		goto is_ipv6;
	}
	if (flag >= 2)
	{
		fprintf(stdout, "%s not ipv4 and not ipv6 format\n", argv[1]);
		goto invalid_format;
	}
is_ip4:
is_ipv6:
	return 0;
invalid_format:
	return 1;
}
