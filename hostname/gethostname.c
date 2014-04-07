#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <linux/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	struct hostent *phost, host;
	char **pptr = NULL, *ptr = NULL;
	char daddr[16];
	int ret;
	while(--argc > 0)
	{
#if 0
		phost = gethostbyname(argv[argc]);
		/* 
		 * $ ./a.out www.hao123.com
		 * official name :hao123.n.shifen.com
		 * alias list : www.hao123.com
		 * h_length :4
		 * host.h_addr_list : 119.75.219.38
		 */
#else
		/* understood */
		/* 
		 * success:
		 * $ ./a.out 192.168.1.111
		 * argv[1]:192.168.1.111
		 * official name :zly-ThinkPad-Edge-E430.local
		 * alias list :
		 * h_length :4
		 * host.h_addr_list : 192.168.1.111
		 * 
		 * failed:
		 * $ ./a.out 123.125.115.150
		 * argv[1]:123.125.115.150
		 * get host name failed
		 * error :Unknown host
		 */
		
		struct in_addr naddr;
		ret = inet_pton(AF_INET, argv[argc], &naddr);
		if (ret < 0)
		{
			fprintf(stdout, "convert error:%s\n", strerror(errno));
		}
		phost = gethostbyaddr(&naddr, sizeof(naddr), AF_INET);
		fprintf(stdout, "argv[%d]:%s\n", argc, argv[argc]);
#endif
		if (!phost)
		{
			fprintf(stdout, "get host name failed\n");
			fprintf(stdout, "error :%s\n", hstrerror(h_errno));
			continue;
		}
		host = *phost;
		fprintf(stdout, "official name :%s\n", host.h_name);
		fprintf(stdout, "alias list :");
		for (pptr = host.h_aliases; *pptr != NULL; pptr++)
			fprintf(stdout, " %s", *pptr);
		putchar('\n');

		fprintf(stdout, "h_length :%d\n", host.h_length);
		bzero(daddr, sizeof(daddr));
		fprintf(stdout, "host.h_addr_list :");
		for (pptr = host.h_addr_list; *pptr != NULL; pptr++)
			fprintf(stdout, " %s", inet_ntop(host.h_addrtype, *pptr, daddr, sizeof(daddr)));
		putchar('\n');
	}

	return 0;
}
