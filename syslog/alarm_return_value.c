#include <stdio.h>
int main(int argc, char *argv[])
{
	int ret;
	ret = alarm(10);
	fprintf(stdout, "ret :%d\n", ret);
	sleep(3);
	ret = alarm(10);
	fprintf(stdout, "ret :%d\n", ret);
	ret = alarm(1);
	fprintf(stdout, "ret :%d\n", ret);
}
