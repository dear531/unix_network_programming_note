#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 1;
	fprintf(stdout, "%s\n", (i == 0) ? "a" : (i == 1) ? "b" : (i == 2) ? "c" : "");
	return 0;
}
