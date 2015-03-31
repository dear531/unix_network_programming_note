#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FTOK_FILE	"ftok_file.ftok"

int main(int argc, char *argv[])
{
	int ret = EXIT_FAILURE;
	key_t key;
	int shmid;
	/* ftok */
	key = ftok(FTOK_FILE, 0xff);
	if (-1 == key) {
		fprintf(stderr, "ftok error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* shmget */
	shmid = shmget(key, sizeof(int), S_IRUSR | S_IWUSR);
	if (-1 == shmid) {
		fprintf(stderr, "shmget error :%s\n",
				strerror(errno));
		goto failure;
	}
	if (-1 == shmctl(shmid, IPC_RMID, NULL)) {
		fprintf(stderr, "shmctl error :%s\n",
				strerror(errno));
		goto failure;
	}
	unlink(FTOK_FILE);
finish:
	exit(ret);
failure:
	ret = EXIT_FAILURE;
	goto finish;
}
