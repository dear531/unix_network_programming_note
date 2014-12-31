#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (2 > argc) {
		fprintf(stdout, "usage :./a.out command1 | command2 ...\n");
		exit(EXIT_FAILURE);
	}
	int i, n;
	char *pp[10];
	for (n = 0, i = 0, pp[0] = &argv[1][0]; 0 != argv[1][i]; i++) {
		if ('|' == argv[1][i]) {
			n++;
			argv[1][i] = 0;
			pp[n] = &argv[1][i+1];
		}
	}
	for (i = 0; n >= i; i++) {
		fprintf(stdout, "pp[%d]:%s\n", i, pp[i]);
	}
	int j;
	int pfd[10][2];
	pid_t pid;
	for (i = 0; n > i; i++) {
		int pfd[10][2];
		pipe(pfd[i]);
		pid = fork();
		if (0 > pid) {
			/* fork error */
		} else if (0 < pid) {
			/* parent */
			if (0 < i) {
				close(pfd[i - 1][1]);
			}
			close(pfd[i][0]);
			dup2(stdout, pfd[i][1]);
			
			close(pfd[i][1]);
			exit(EXIT_SUCCESS);
		} else {
			/* child */
			if (1 < i)
				close(pfd[i -1][0]);
			close(pfd[i][1]);
		}
	}
	
	return 0;
}
