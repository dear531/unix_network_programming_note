#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	char strn[] = "123456l";
	char *store;
	int num = 0;
	num = strtol(strn, &store, 10);
	switch (num)
	{
		case ERANGE:
			fprintf(stdout, "numaric range wide\n");
			break;
		default:
			fprintf(stdout, "numaric convert :%d\n", num);
			break;
	}
	fprintf(stdout, "strn[%ld]:%p\nstore:%p\n", store - strn, &strn[store - strn], store);
	return 0;
}
