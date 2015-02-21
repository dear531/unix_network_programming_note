/*
 * usege :
 * $ ./semcreate -e -i 1 test
 * $ ls dev/shm/sem.test (file already exists)
 * $ ./semunlink test
 * $ ls //dev/shm/ (file already remove)
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	/* init variable */
	int flags = O_RDWR | O_CREAT;
	int value;
	char ch;
	for ( ; -1 != (ch = getopt(argc, argv, "ei:")); ) {
	       switch (ch) {
		       case 'e':
			       flags |= O_EXCL;
			       break;
		       case 'i':
			       value = (int)strtol(optarg, NULL, 10);
			       break;
		       default:
			       fprintf(stdout, "wrong option\n");
	       }
	}
#if 0
       sem_t *sem_open(const char *name, int oflag,
                       mode_t mode, unsigned int value);
#endif
	/* sem_open */
	if (argc - 1 != optind) {
		fprintf(stderr, "command-line option error\n");
		fprintf(stdout, "usage : semcrate [-e] [-i seminitvalue] name\n");
		exit(EXIT_FAILURE);
	}
#define FILE_MODE	 (S_IRUSR | S_IWUSR)
	sem_t *sem;
	sem = sem_open(argv[optind], flags, FILE_MODE, value);
	if (SEM_FAILED == sem) {
		fprintf(stderr, "sem_open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* sem_close */
	int ret;
	ret = sem_close(sem);
	if (-1 == ret) {
		fprintf(stderr, "sem_close error:%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}
