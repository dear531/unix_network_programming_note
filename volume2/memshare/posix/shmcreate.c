#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 

#define MEM_SHARE_FILE	"memshare.share"

int main(int argc, char *argv[])
{
	int fd;
	int ret = EXIT_SUCCESS;
	int flag = O_RDWR;
	int len = 10;
	/* shm_open */
shm_open_again:
	fd = shm_open(MEM_SHARE_FILE, flag, S_IRUSR | S_IWUSR);
	if (-1 == fd && ENOENT == errno) {
		flag |= O_CREAT;
		goto shm_open_again;
	} else if (-1 == fd) {
		fprintf(stderr, "shm_open error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* ftruncate */
	if (-1 == ftruncate(fd, len)) {
		fprintf(stderr, "ftruncate error :%s\n",
				strerror(errno));
		goto failure;
	}
#if 0
       void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
#endif
       if (MAP_FAILED == mmap(NULL, len, PROT_READ | PROT_WRITE,
			       MAP_SHARED, fd, 0)) {
	       fprintf(stderr, "mmap error :%s\n",
			       strerror(errno));
	       goto failure;
       }
       if (0 != (flag & O_CREAT)) {
	       fprintf(stdout, "memory share success\n");
       } else {
	       fprintf(stdout, "memory share alread exists\n");
       }
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
