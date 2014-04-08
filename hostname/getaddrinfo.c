#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


	int
main(int argc, char *argv[])
{
	char *psname = "baidu.com";
	char *psserv = "http";
	struct addrinfo *raddinf;
	int ret;
	ret = getaddrinfo(psname, psserv, NULL, &raddinf);
	if (ret < 0)
	{
		fprintf(stdout, "getaddrinfo error :%s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}
#if 0
	struct addrinfo {
		int              ai_flags;
		int              ai_family;
		int              ai_socktype;
		int              ai_protocol;
		size_t           ai_addrlen;
		struct sockaddr *ai_addr;
		char            *ai_canonname;
		struct addrinfo *ai_next;
	};
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
	fprintf(stdout, "ai_flags:%d\n", raddinf->ai_flags);
	fprintf(stdout, "ai_family:%d, AF_IENT:%d\n", raddinf->ai_family, AF_INET);
	fprintf(stdout, "ai_canonname :%s\n", raddinf->ai_canonname);
	struct sockaddr_in* p = NULL;
	p = (struct sockaddr_in*)raddinf->ai_addr;
	fprintf(stdout, "ai_addr->addr:%s\n", inet_ntoa(p->sin_addr));
#if 0
	ai_socktype;
	ai_protocol;
#endif

	return 0;
}
