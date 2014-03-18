#include <stdio.h>
#include <strings.h>

#include "bit_check.h"

int main(int argc, char *argv[])
{
	int n = 0x010203f4;
	int m;
	printf("%#010x\n", n);
	bit_check(&n, sizeof(n));

	/*  test bzero */
	bzero(&n, sizeof(n) - 3);
	printf("%#010x\n", n);
	bit_check(&n, sizeof(n));

	/* test bcopy */
	n = 0x01020304;
	bzero(&m, sizeof(m));
	bit_check(&m, sizeof(m));
	bcopy(&n, &m, sizeof(m) - 2);
	bit_check(&m, sizeof(m));
	int i;
	char *p = (char *)&m;
	for (i = 0; i < sizeof(n);i++)
		printf("p[%d]:%d\n", i, p[i]);
	int ret;
	ret = bcmp(&n, &m, sizeof(m));
	printf("bcmp(n, m):%d\n", ret);
	ret = bcmp(&m, &n, sizeof(m));
	printf("bcmp(m, n):%d\n", ret);
	bcopy(&n, &m, sizeof(m));
	ret = bcmp(&m, &n, sizeof(m));
	printf("recopy all\nbcmp(m, n):%d\n", ret);
	

	return 0;
}
