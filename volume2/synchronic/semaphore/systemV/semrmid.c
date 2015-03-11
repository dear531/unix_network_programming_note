#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char *argv[])
{
	key_t key;
	int semid;
	if (2 != argc) {
		fprintf(stderr, "usage : semrmid key\n");
		exit(EXIT_FAILURE);
	}
	key = strtol(argv[argc - 1], NULL, 10);
	if ((LONG_MAX == key || LONG_MIN == key) && ERANGE == errno) {
		fprintf(stderr, "key overflow\n");
		exit(EXIT_FAILURE);
	}
	semid = semget(key, 0, 0);
	if (-1 == semid) {
		fprintf(stderr, "semget error : %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (-1 == semctl(semid, 0, IPC_RMID)) {
		fprintf(stderr, "semctl error : %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}
