#include <stdio.h>
#include <signal.h>
#include <semaphore.h>

sem_t shared;

int main(int argc, char *argv[])
{

	signal(SIGCHLD, SIG_IGN);
#if 0	
	int sem_init(sem_t *sem, int pshared, unsigned
			int value);
#endif
	sem_init(&shared, 1, 1);
	pid_t pid;
	int i;
	pid = fork();
	if (0 > pid) {
		/* fork erro */
	} else if (0 < pid) {
		/* parent */
		for (i = 0; 10 > i; i++) {
			sem_post(&shared);
			fprintf(stdout, "here parent :%d\n", i);
			sleep(1);
		}
	} else {
		/* child */
		for (i = 0; 10 > i; i++) {
			sem_wait(&shared);
			fprintf(stdout, "here child :%d\n", i);
			sleep(1);
		}
	}
	sem_destroy(&shared);
	return 0;
}
