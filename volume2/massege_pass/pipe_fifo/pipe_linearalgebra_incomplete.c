/*
 * run:
# ./a.out " 1 | 2 | 3 "
pp[0]: 1 
pp[1]: 2 
pp[2]: 3 
i:1, buff:1234

 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
	char buff[64] = {0};
	for (i = 0; n > i; i++) {
		int pfd[10][2];
		pipe(pfd[i]);
		pid = fork();
		if (0 > pid) {
			/* fork error */
		} else if (0 == pid) {
			/* child */
			if (0 < i) {
				close(pfd[i - 1][1]);
			}
			close(pfd[i][0]);
			if (1 < i) {
				close(pfd[i -2][0]);
				close(pfd[i - 1][1]);
			}
			if(n - 1 == i) {
			/* end */
				read(pfd[i - 1][0], buff, sizeof(buff));
				fprintf(stdout, "i:%d, buff:%s\n", i, buff);
			} else {
			/* mide */
				read(pfd[i -1][0], buff, sizeof(buff));
				write(pfd[i - 1][1], buff, strlen(buff));
			}
			
			close(pfd[i][1]);
			if (0 < i)
				close(pfd[i -1][0]);
			exit(EXIT_SUCCESS);
		} else {
			/* parent */
			if (0 == i) {
			/* begin */
				write(pfd[i][1], "1234", sizeof("1234"));
			}
		}
	}
	sleep(2);
	
	return 0;
}
