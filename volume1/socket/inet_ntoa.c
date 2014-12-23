#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>



int main(void)
{
#if 0
           struct sockaddr_in {
               sa_family_t    sin_family; /* address family: AF_INET */
               in_port_t      sin_port;   /* port in network byte order */
               struct in_addr sin_addr;   /* internet address */
           };
#endif
	char *p = NULL;
	struct sockaddr_in s;
	p = inet_ntoa(s.sin_addr);
	fprintf(stdout, "%s\n", p);
	return 0;
}
