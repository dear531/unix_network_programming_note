#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHARE_FILE	"1.txt"
#define CONTEXT		"1111111111"

int main(int argc, char *argv[])
{
	/* open file */
	int fd = -1;
	fd = open(SHARE_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (0 > fd) {
		fprintf(stderr, "open file :%s error %s\n",
				SHARE_FILE, strerror(errno));
		exit(EXIT_FAILURE);
	}
	write(fd, CONTEXT, sizeof(CONTEXT));
	
#if 0
       void *mmap(void *start, size_t length, int prot, int flags,
                  int fd, off_t offset);
#endif
#define SHARE_FILE_LENGTH	10
	/* mmap */
	char *pf;
	pf = mmap(NULL, SHARE_FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (MAP_FAILED == pf) {
	   fprintf(stderr, "mmap error :%s\n",
			   strerror(errno));
	   exit(EXIT_FAILURE);
	}
	int i;
	for (i = 0; SHARE_FILE_LENGTH > i; i++) {
		pf[i] = '0' + i;
	}
	/* close file */
	if (0 < fd) {
		close(fd);
		fd = -1;
	}

	return 0;
}
