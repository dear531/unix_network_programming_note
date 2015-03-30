#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MEM_SHARE_FILE	"memshare.share"

int main(int argc, char *argv[])
{
	if (-1 == shm_unlink(MEM_SHARE_FILE)) {
		fprintf(stderr, "shm_unlink error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
}
