#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
							   (Linux-specific) */
};
int main(int argc, char *argv[])
{
	int semid;
	key_t key;
	union semun arg;
	struct semid_ds buf;
	unsigned short nsems;
	struct sembuf *sops = NULL;
	int i;
	if (2 >= argc) {
		fprintf(stdout, "usage : semopt key operation ...\n");
		goto failure;
	}
	key = strtol(argv[1], NULL, 10);
	if ((LONG_MIN == key ||LONG_MAX == key) && ERANGE == errno) {
		fprintf(stderr, "overflow key\n");
		goto failure;
	}
	semid = semget(key, 0, 0);
	if (-1 == semid) {
		fprintf(stderr, "line %d semget error :%s\n",
				__LINE__, strerror(errno));
		goto failure;
	}
	arg.buf = &buf;
	if (-1 == (semctl(semid, 0, IPC_STAT, arg))) {
		fprintf(stderr, "semctl error :%s\n",
				strerror(errno));
		goto failure;
	}
	nsems = argc - 2;
	if (nsems > arg.buf->sem_nsems) {
		fprintf(stderr, "operation too longer\n");
		goto failure;
	}
	sops = malloc(sizeof(*sops) * nsems);
	if (NULL == sops) {
		fprintf(stderr, "malloc error :%s\n",
				strerror(errno));
		goto failure;
	}
#if 0
           unsigned short sem_num;  /* semaphore number */
           short          sem_op;   /* semaphore operation */
           short          sem_flg;  /* operation flags */
#endif
	for (i = 0; nsems > i; i++) {
		sops[i].sem_num	= i;
		sops[i].sem_op	= strtol(argv[2 + i], NULL, 10);
		sops[i].sem_flg	= IPC_NOWAIT;
	}
	if (-1 == (semop(semid, sops, nsems))) {
		fprintf(stderr, "semop error :%s\n",
				strerror(errno));
		goto failure;
	}
	return 0;
failure:
	if (NULL != sops) {
		free(sops);
		sops = NULL;
	}
	exit(EXIT_FAILURE);
}
