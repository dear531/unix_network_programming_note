#include <stdio.h>

#define result(set) (*(set) = 0, 1)

int main(int argc, char *argv[])
{
	int set;
	fprintf(stdout, "result:%d\n", result(&set));
	return 0;
}
