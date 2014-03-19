#include <stdio.h>

int main(int argc, char *argv[])
{
	int ret;
	ret = alarm(10);
	fprintf(stdout, "ret:%d\n", ret);
	sleep(5);
	ret = alarm(3);
	fprintf(stdout, "ret:%d\n", ret);
	sleep(10);
	return 0;
}
