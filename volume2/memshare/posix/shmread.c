#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MEM_SHARE_FILE	"memshare.share"
#define MMAP_LENTH	10

int main(int argc, char *argv[])
{
#if 0
	int shm_open(const char *name, int oflag, mode_t mode);
#endif
	int ret = EXIT_SUCCESS;
	int fd;
	char *p = NULL;
	int len = MMAP_LENTH;
	int i;
	fd = shm_open(MEM_SHARE_FILE, O_RDWR, 0);
	if (-1 == fd) {
		fprintf(stderr, "shm_open erro :%s\n",
				strerror(errno));
		goto failure;
	}
#if 0
       void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
#endif
	p = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
	if (MAP_FAILED == p) {
		fprintf(stderr, "mmap error :%s\n",
				strerror(errno));
		goto failure;
	}
	for (i = 0; MMAP_LENTH > i; i++) {
		putchar(p[i]);
	}
	putchar('\n');
finish:
	if (-1 != fd) {
		close(fd);
		fd = -1;
	}
	exit(ret);
failure:
	ret = EXIT_FAILURE;
	goto finish;
}
