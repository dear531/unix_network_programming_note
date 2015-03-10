#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <limits.h>


int main(int argc, char *argv[])
{
	char ch;
	int flag = IPC_CREAT;
	key_t key;
	int nsems; 
	int semid;
	if (3 > argc) {
		goto failed;
	}
	while (-1 != (ch = getopt(argc, argv, "e"))) {
		switch (ch) {
			case 'e':
				flag |= IPC_EXCL;
				break;
			default:
				goto failed;
		}
	}
	if (argc - 2 != optind) {
			goto failed;
	}
	key = strtol(argv[argc - 2], NULL, 10);
	if (ERANGE == errno && (LONG_MAX == key || LONG_MIN == key)) {
		fprintf(stderr, "key overfolw\n");
		exit(EXIT_FAILURE);
	}
	nsems = strtol(argv[argc - 1], NULL, 10);
	if (ERANGE == errno && (LONG_MAX == nsems || LONG_MIN == nsems)) {
		fprintf(stderr, "nsems overfolw\n");
		exit(EXIT_FAILURE);
	}
#if 0
		int semget(key_t key, int nsems, int semflg);
#endif
	semid = semget(key, nsems, flag);
	if (-1 == semid) {
		fprintf(stderr, "semget error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 0;
failed:
		fprintf(stderr, "usage : semcreate [-e] key_t nsems\n");
		exit(EXIT_FAILURE);
}
