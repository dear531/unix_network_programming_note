#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>



void sig_chld(int signum)
{
	while(waitpid(-1, NULL, WNOHANG) > 0)
		fprintf(stdout, "perfrom sig_chld signum :%d\n", signum);
	return;
}

int main(int argc, char *argv[])
{
	int i;
	//signal(SIGCHLD, SIG_IGN);
#if 0
	struct sigaction act;

	act.sa_handler = SIG_IGN;
	act.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &act, NULL);
#endif
	int fd;
	fd = open("1.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0)
		fprintf(stdout, "open file 1.txt error:%s\n", strerror(errno));
	fprintf(stdout, "open file fd :%d\n", fd);

	int opt;
	opt = fcntl(fd, F_GETFD);
	if (opt < 0)
		fprintf(stdout, "get fd error: %s\n", strerror(errno));
	opt |= FD_CLOEXEC;

	int ret;
	ret = fcntl(fd, F_SETFD, opt);
	if (ret < 0)
		fprintf(stdout, "set fd error: %s\n", strerror(errno));

	pid_t pid;
	char buf[] = "\n";
	char str_fd[2];
	for (i = 0; i < 5; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			fprintf(stdout, "inherit fd :%d\n", fd);
			buf[0] = '0' + i;
			ret = write(fd, buf, sizeof(buf));
			if (ret < 0)
				fprintf(stdout, "child %d write failed\n", getpid());
			//fsync(fd);
			sprintf(str_fd, "%d", fd);
			/* in children process in exec file descriptor auto close  */
			ret = execl("exec","./exec", str_fd, NULL);
			if (ret < 0)
				fprintf(stdout, "execute call failed :%s\n", strerror(errno));
			close(fd);
			exit(EXIT_SUCCESS);
		}
	}
	//for (i = 0; i < 200; i++)
	ret = write(fd, buf, sizeof(buf));
	if (ret < 0)
		fprintf(stdout, "perent write failed\n");
	sprintf(str_fd, "%d", fd);
	/* on perent exec also close file descriptor */
	ret = execl("exec","./exec", str_fd, NULL);
	if (ret < 0)
		fprintf(stdout, "execute call failed :%s\n", strerror(errno));
	fprintf(stdout, "appear\n");
	//fsync(fd);
	sleep(5);
	while(1)
		sleep(1);
	close(fd);

	return 0;
}
