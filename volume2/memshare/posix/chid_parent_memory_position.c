#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHARE_MEMORY_FILE	"shm_file.share"
#define MMAP_FILE		"mmap_file.mmap"

int main(int argc, char *argv[])
{
	/* initialized data */
	int shmfd = -1, mmapfd = -1;
	int ret = EXIT_SUCCESS;
	int *pmmap = NULL, *pshm = NULL;
	pid_t pid;
	/* shm_open for share*/
	shmfd = shm_open(SHARE_MEMORY_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (-1 == shmfd) {
		fprintf(stderr, "shm_open error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* ftruncate for share*/
	if (-1 == ftruncate(shmfd, sizeof(int))) {
		fprintf(stderr, "ftruncate error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* open for mmap*/
	mmapfd = open(MMAP_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (-1 == mmapfd) {
		fprintf(stderr, "open error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* ftruncate for mmap */
	if (-1 == ftruncate(mmapfd, sizeof(int))) {
		fprintf(stderr, "ftruncate error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* fork */
	pid = fork();
	if (-1 == pid) {
		/* fork error */
	}
	/* mmap for share*/
#if 0
	void *mmap(void *addr, size_t length, int prot, int flags,
		  int fd, off_t offset);
#endif
	pmmap = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
			MAP_SHARED, mmapfd, 0);
	if (NULL == pmmap) {
		fprintf(stderr, "mmap for mmap error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* mmap for mmap */
	pshm  = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
			MAP_SHARED, shmfd, 0);
	if (NULL == pshm) {
		fprintf(stderr, "mmap for share error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* opretion */
	if (0 == pid) {
		fprintf(stdout, "child pmmap\t:%p, pshm :%p\n",
				pmmap, pshm);
		sleep(5);
		fprintf(stdout, "read pmmap :%d, pshm :%d\n",
				*pmmap, *pshm);
	} else if (0 < pid) {
		fprintf(stdout, "parent pmmap\t:%p, pshm :%p\n",
				pmmap, pshm);
		*pmmap = 777;
		*pshm = 666;
		waitpid(pid, NULL, 0);
		
	}
	/* close for share */
	if (-1 == shmfd) {
		close(shmfd);
		shmfd = -1;
	}
	/* close for mmap */
	if (-1 == mmapfd) {
		close(mmapfd);
		mmapfd = -1;
	}
	/* munmap for mmap and share*/
finish:
	if (0 == pid) {
		goto child_finish;
	}
	/* shm_unlink */
	if (-1 == shm_unlink(SHARE_MEMORY_FILE)) {
		fprintf(stderr, "shm_unlink error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* unlink for mmap */
	unlink(MMAP_FILE);
child_finish:
	exit(ret);
failure:
	ret = EXIT_SUCCESS;
	goto finish;
}
