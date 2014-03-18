#include <stdio.h>
#include <arpa/inet.h>
#include <linux/types.h>

#include "bit_check.h"



int
main(int argc, char *argv[])
{
/*
int inet_pton(int af, const char *src, void *dst);
*/
	struct in_addr tmp;
	char buf[sizeof(struct in_addr)] = {0};
	char ip[INET_ADDRSTRLEN] = {0};
	int i;
	__be32 ipbe32;
	printf("sizeof(struct in_addr):%ld\n",
			sizeof(struct in_addr));
	inet_pton(AF_INET, "127.127.1.1", buf);
	
	inet_ntop(AF_INET, buf, ip, INET_ADDRSTRLEN);
	printf("ip:%s\n", ip);

	inet_pton(AF_INET, "127.0.0.1", &tmp.s_addr);
	bit_check(&tmp.s_addr, sizeof(tmp.s_addr));
	
	inet_pton(AF_INET, "127.0.0.1", &ipbe32);
	bit_check(&ipbe32, sizeof(ipbe32));
	
	return 0;
}
