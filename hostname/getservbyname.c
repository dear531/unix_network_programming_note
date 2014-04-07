#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/types.h>

int main(int argc, char *argv[])
{
	struct servent *sp, ss;
	int i, j;
	__be16 port;
	j = 0;
	while (++j < argc)
	{
#if 1
		sp = getservbyname(argv[j], NULL);
#else
		port = htons(atoi(argv[j]));
		sp = getservbyport(port, "tcp");
		fprintf(stdout, "port size :%ld\n", sizeof(ss.s_port));
#endif
		if (sp == NULL)
		{
			fprintf(stdout, "get server info error :%s\n", hstrerror(h_errno));
			continue;
		}
#if 0
		struct servent {
			char  *s_name;       /* official service name */
			char **s_aliases;    /* alias list */
			int    s_port;       /* port number */
			char  *s_proto;      /* protocol to use */
		}

#endif
		ss = *sp;
		fprintf(stdout, "server name :%s\n", ss.s_name);
		fprintf(stdout, "server alias:");
		for (i = 0; ss.s_aliases[i] != NULL; i++)
			fprintf(stdout, "%s", ss.s_aliases[i]);
		putchar('\n');
		fprintf(stdout, "server port :%d\n", ntohs(ss.s_port));
		fprintf(stdout, "server protocol :%s\n", ss.s_proto);
	}
	return 0;
}
