#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(void)
{
	int ret;
	char *commond[] = {"ls", "-al", NULL,};
	pid_t pid;
	int status;
	int i;
	for (i = 0; i < 5; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			if (i)
				fprintf(stdout, "=========\n");
			fprintf(stdout, "i:%d\n", i);
			sleep(1);
			switch (i)
			{
				case 0: 
					ret = execvp("NLLL", commond);
					goto cser;
				case 1:
					ret = execlp("ls-a", "ls", "-al", NULL);
					goto cser;
				case 2:
					ret = execl("/bin/ls", "ls", "-al", NULL);
					goto cser;
				case 3:
					ret = execv("/bin/ls", commond);
					goto cser;
				case 4:
					ret = execv("/bin/ls", commond);
					goto cser;
cser:
				if (ret < 0)
					fprintf(stdout, "error:%s\n", strerror(errno));
			}
			break;
		}
		else if (pid > 0)
		{
			wait(&status);
		}
		else
		{
			fprintf(stdout, "error:%s\n", strerror(errno));
		}
	}
	return 0;
}
