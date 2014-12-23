#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int n = 3;
	fprintf(stdout, "n :%ld\n", (long)sizeof(n));

	return 0;
}
