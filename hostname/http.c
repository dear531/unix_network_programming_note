#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

int main(int argc, char *argv[])
{
	/* get server */
	struct servent *ps, sts;
	ps = getservbyname(argv[1], NULL);
	if (ps == NULL)
	{
		fprintf(stdout, "get server info error :%s\n", hstrerror(h_errno));
		exit(EXIT_FAILURE);
	}
#if 0
	struct servent {
		char  *s_name;       /* official service name */
		char **s_aliases;    /* alias list */
		int    s_port;       /* port number */
		char  *s_proto;      /* protocol to use */
	}
#endif
	sts = *ps;
	if (bcmp(sts.s_proto, "tcp", sizeof("tcp")) != 0)
	{
		fprintf(stdout, "%s is %s, only tcp protocol\n",
				argv[1], sts.s_proto);
		exit(EXIT_FAILURE);
	}
	struct hostent *ph, sth;
	ph = gethostbyname(argv[2]);
	if (ph == NULL)
	{
		fprintf(stdout, "get host info error :%s\n", hstrerror(h_errno));
		exit(EXIT_FAILURE);
	}
	/* only exist udp protocol: tftp */
#if 0
	struct hostent {
		char  *h_name;            /* official name of host */
		char **h_aliases;         /* alias list */
		int    h_addrtype;        /* host address type */
		int    h_length;          /* length of address */
		char **h_addr_list;       /* list of addresses */
	}
#endif
	/* get address */
	int i;
	sth = *ph;
#define DEBUG	0
#if DEBUG
	char htip[16];	/* host temp ip */
	for (i = 0; sth.h_addr_list[i] != NULL && i < sth.h_length; i++)
		fprintf(stdout, "addr_list :%s\n", inet_ntop(sth.h_addrtype, sth.h_addr_list[i], htip, sizeof(htip)));
#endif
	/* socket */
	int confd;
	confd = socket(sth.h_addrtype, SOCK_STREAM, IPPROTO_TCP);
	/* bind */
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
	i = 0;
	struct sockaddr_in saddr;
	int ret;
again:
	saddr.sin_family = sth.h_addrtype;
	saddr.sin_port = sts.s_port;
	bcopy(sth.h_addr_list[i], &saddr.sin_addr.s_addr, sizeof(saddr.sin_addr.s_addr));
#if DEBUG
	fprintf(stdout, "saddr :%s, port: %d\n",
			inet_ntop(sth.h_addrtype, &saddr.sin_addr, htip, sizeof(htip)),
			ntohs(saddr.sin_port));
#endif
	ret = connect(confd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret < 0 && ++i < sth.h_length)
	{
		fprintf(stdout, "next addr\n");
		goto again;
	}
	char *sbuf[] = {
			"GET / HTTP/1.1\r\r",
			"Host: www.hao123.com\r\r",
			"Connection: keep-alive\r\r",
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\r",
			"User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/32.0.1700.107 Chrome/32.0.1700.107 Safari/537.36\r\r",
			"Accept-Encoding: gzip,deflate,sdch\r\r",
			"Accept-Language: zh-CN,zh;q=0.8\r\r",
			"Cookie: bdshare_firstime=1370829194578; BAIDUID=2D832A914EFF6E341F16E76A489D03CC:FG=1; FLASHID=0EB13682FDD63510AE5353808E9152C5:FG=1; ftip=1; Hm_lvt_22661fc940aadd927d385f4a67892bc3=1388973790,1390724801; static=1; HUM=; HUN= ",
			};
	//sprintf(sbuf, "", argv[2]);
	for (i = 0; i < sizeof(sbuf) / sizeof(*sbuf); i++)
	{
		send(confd, sbuf[i], strlen(sbuf[i]), 0);
		fprintf(stdout, "sbuf[%d]:%s\n", i, sbuf[i]);
	}
	int n;
	char rbuf[1024];
	while(bzero(rbuf, sizeof(rbuf)),(n = recv(confd, rbuf, sizeof(sbuf), 0)) > 0)
		fprintf(stdout, "%s", rbuf);

	return 0;	
}
