#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FTOK_FILE	"ftok_file.ftok"
int main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;
	key_t key;
	int shmid;
	unsigned char *ptr = NULL;
	struct shmid_ds buf;
	int i;
	/* ftok */
	key = ftok(FTOK_FILE, 0xff);
	if (-1 == key) {
		fprintf(stderr, "ftok error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* shmget */
	shmid = shmget(key, 0, S_IRUSR | S_IWUSR);
	if (-1 == shmid) {
		fprintf(stderr, "shmget error :%s\n",
				strerror(errno));
		goto failure;
	}
	ptr = shmat(shmid, NULL, 0);
	if (((void *)-1) == ptr) {
		fprintf(stderr, "shmat error :%s\n",
				strerror(errno));
		goto failure;
	}
	if (-1 == shmctl(shmid, IPC_STAT, &buf)) {
		fprintf(stderr, "shmctl error :%s\n",
				strerror(errno));
		goto failure;
	}
	for (i = 0; buf.shm_segsz > i; i++) {
		ptr[i] = i % 256;
	}
finish:
	exit(ret);
failure:
	ret = EXIT_FAILURE;
	goto finish;
}
