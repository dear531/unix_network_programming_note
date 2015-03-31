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
	if (-1 == open(FTOK_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) {
		fprintf(stderr, "open error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* ftok */
	key = ftok(FTOK_FILE, 0xff);
	if (-1 == key) {
		fprintf(stderr, "ftok error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* shmget */
	if (-1 == shmget(key, BUFSIZ,
			IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) {
		fprintf(stderr, "shmget error :%s\n",
				strerror(errno));
		goto failure;
	}
finish:
	exit(ret);
failure:
	ret = EXIT_FAILURE;
	goto finish;
}
