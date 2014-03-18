#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>

#include "bit_check.h"
#if 0
/* Internet address. */
struct in_addr {
	__be32	s_addr;
};
#endif

#define MAXIP	sizeof("255.255.255.255")
int main(int argc, char *argv[])
{
	char ip[] = "127.0.0.1";
	struct in_addr tmp;
	char p[MAXIP];
	char *tmp_p = NULL;
	char *tmp_p2 = NULL;
	char *tmp_p3 = NULL;

	printf("MAXIP:%ld\n", MAXIP);
	inet_aton("192.168.1.1", &tmp);
	bit_check(&tmp.s_addr, sizeof(tmp.s_addr));

	tmp_p2 = inet_ntoa(tmp);
	bcopy(tmp_p2, p, strlen(tmp_p2) + 1);
	printf("p:%s\n", p);

	tmp.s_addr = inet_addr("192.168.1.1");
	bit_check(&tmp.s_addr, sizeof(tmp.s_addr));
	
	tmp_p = inet_ntoa(tmp);
	bcopy(tmp_p, p, strlen(tmp_p) + 1);
	printf("p:%s\n", p);

	tmp.s_addr = inet_network("192.168.1.1");
	bit_check(&tmp.s_addr, sizeof(tmp.s_addr));
	tmp.s_addr = inet_network("255.255.255.255");
	bit_check(&tmp.s_addr, sizeof(tmp.s_addr));
	tmp.s_addr = inet_addr("255.255.255.255");
	bit_check(&tmp.s_addr, sizeof(tmp.s_addr));

	

	return 0;
}
