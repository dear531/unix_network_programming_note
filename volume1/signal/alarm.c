#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	fprintf(stdout, "here's alarm 10\n");
	alarm(10);
	fprintf(stdout, "here is sleep 20\n");
	sleep(20);
	return 0;
}
