#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char *argv[])
{
	if (2 > argc) {
		fprintf(stderr, "usage :./a.out fifofilename\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "fifo size :%ld\n",
			pathconf(argv[1], _PC_PIPE_BUF));
	fprintf(stdout, "maxinum  file descriptor :%ld\n",
			sysconf(_SC_OPEN_MAX));

	return 0;
}
