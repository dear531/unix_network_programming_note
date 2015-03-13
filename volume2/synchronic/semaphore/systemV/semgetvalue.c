#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;
};

int main(int argc, char *argv[])
{
	/* check arg command line */
	key_t key;
	int semid;	
	union semun arg;
	struct semid_ds buf;
	int nsems;
	int i;
	unsigned short *array;
	if (2 != argc) {
		fprintf(stderr, "usage : semgetvalue key\n");
		goto failure;
	}
	/* strtol for convert from string to numaric */
	key = strtol(argv[1], NULL, 10);
	if ((LLONG_MIN == key || LLONG_MAX == key) && ERANGE == errno) {
		fprintf(stderr, "strtol error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* semget */
	semid = semget(key, 0, 0);
	if (-1 == semid) {
		fprintf(stderr, "semget error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* semctl get number array of value */
	arg.buf = &buf;
	if (-1 == semctl(semid, 0, IPC_STAT, arg)) {
		fprintf(stderr, "semctl error :%s\n",
				strerror(errno));
		goto failure;
	}
	nsems = arg.buf->sem_nsems;
	array = calloc(nsems, sizeof(*array));
	if (NULL == array) {
		fprintf(stderr, "calloc error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* semctl get value */
	arg.array = array;
	if (-1 == semctl(semid, 0, GETALL, arg)) {
		fprintf(stderr, "semctl error :%s\n",
				strerror(errno));
		goto failure;
	}
	/* show array */
	for (i = 0; nsems > i; i++) {
		fprintf(stdout, "array[%d]:%d\n",
				i, array[i]);
	}
	return 0;
failure:
	exit(EXIT_FAILURE);
}
