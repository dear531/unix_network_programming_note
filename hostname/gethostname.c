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
	struct hostent *pptmp = NULL;
	char daddr[16];
	int ret;
	int flag = 0;
	while(--argc > 0)
	{
#if 1
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
		if (!flag++)
		{
			pptmp = phost;
			fprintf(stdout, "assign argv[%d]:%s\n", argc, argv[argc]);
		}
		fprintf(stdout, "**argv[%d]:%s name :%s addr :%p\n",
				argc, argv[argc], pptmp->h_name, pptmp);
		/* 
		 * addr not change, but pointer contain change
		 * $ ./a.out www.hao123.com www.baidu.com
		 * ......
		 * **argv[2]:www.baidu.com name :www.a.shifen.com addr :0x7f6075fd2f20 //first show
		 * ......
		 * **argv[1]:www.hao123.com name :hao123.n.shifen.com addr :0x7f6075fd2f20 // second show
		 * ......
		 * this is overwritten, should peed copy.
		 */
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
