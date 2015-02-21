#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (2 != argc) {
		fprintf(stderr, "usage : semunlink name\n");
		exit(EXIT_FAILURE);
	}
	int ret;
	ret = sem_unlink(argv[1]);
	if (-1 == ret) {
		fprintf(stderr, "sem_unlink error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
