#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

int main(int argc, char *argv[])
{
	key_t key;
	int semid;
	union semun arg;
    struct semid_ds buf;
	arg.buf = &buf;
	int i;
	unsigned long nsems;
	unsigned short *ptr;
	if (2 != argc) {
		fprintf(stderr, "usage : semsetvalue key\n");
		goto failure;
	}
	key = strtol(argv[1], NULL, 10);
	if ((LLONG_MIN == key || LLONG_MAX == key) && ERANGE == errno) {
		fprintf(stderr, "key overflow\n");
		goto failure;
	}
	semid = semget(key, 0, 0);
	if (-1 == semid) {
		fprintf(stderr, "semget error : %s\n",
				strerror(errno));
		goto failure;
	}
	if (-1 == semctl(semid, 0, IPC_STAT, arg)) {
		fprintf(stderr, "line : %d semctl error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	}
	nsems = arg.buf->sem_nsems;
	ptr = calloc(nsems, sizeof(*ptr));
	if (NULL == ptr) {
		fprintf(stderr, "calloc error :%s\n",
				strerror(errno));
		goto failure;
	}
	arg.array = ptr;
	for (i = 0; nsems > i; i++) {
		ptr[i] = i;
	}
	if (-1 == semctl(semid, 0, SETALL, arg)) {
		fprintf(stderr, "line %d semctl error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	}
	if (NULL != ptr)
		free(ptr);
	return 0;
failure:
	if (NULL != ptr)
		free(ptr);
	exit(EXIT_FAILURE);
}
