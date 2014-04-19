#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int read_fd(int fd, void *ptr, int nbytes, int *recv_fd)
{
	struct iovec iov[1];
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;

	union{
	struct cmsghdr cm;
	char control[CMSG_SPACE(sizeof(int))];
	}control_un;

	struct msghdr msg;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = sizeof(iov) / sizeof(*iov);
	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	int n;
	n = recvmsg(fd, &msg, 0);
	fprintf(stdout, "n :%d\n", n);
	if (n < 0)
	{
		fprintf(stdout, "recvmsg error :%s\n", strerror(errno));
		goto failed;
	}
	else if (n == 0)
	{
		fprintf(stdout, "recvmsg error :%s\n", strerror(errno));
	}
	struct cmsghdr *pmsg;
	pmsg = CMSG_FIRSTHDR(&msg);
#if 1
	if (pmsg == NULL || pmsg->cmsg_len != CMSG_LEN(sizeof(int)))
		goto failed;
	if (pmsg->cmsg_level != SOL_SOCKET || pmsg->cmsg_type != SCM_RIGHTS)
		goto failed;
#endif
	/* maybe same number file discrptor with sended fd function */
	fprintf(stdout, "*((int *)CMSG_DATA(pmsg)) :%d\n", *((int *)CMSG_DATA(pmsg)));
	*recv_fd = *((int *)CMSG_DATA(pmsg));
	return n;
failed:
	return -1;
}
void sighandle(int signum)
{
	int saverr;
	saverr = errno;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		fprintf(stdout, "chlid terminal\n");
	errno = saverr;
	return;
}
int main(int argc, char *argv[])
{
	if (signal(SIGCHLD, sighandle) == SIG_ERR)
		fprintf(stdout, "set sigchld error :%s\n", strerror(errno));
	/* socketpair */
	int inv[2];
	if ((socketpair(AF_LOCAL, SOCK_STREAM, 0, inv)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* fork */
	pid_t pid;
	pid = fork();
	char strfd[5];
	char ptr[1024];
	int recv_fd;
/* see below defined macro of failed cause */
#define FAIL_STATUS	0
#if FAIL_STATUS
	char buf[1024];
	int i, n;
#endif
	if (pid < 0)
	{ /* fork error */
		fprintf(stdout, "fork error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{ /* child */
		close(inv[0]);
		bzero(strfd, sizeof(strfd));
		snprintf(strfd, sizeof(strfd), "%d", inv[1]);
		execl("getfd", "./getfd", strfd, NULL);
		close(inv[1]);
	}
	else
	{ /* prenet */
		close(inv[1]);
#if FAIL_STATUS
		sleep(1);
		bzero(buf, sizeof(buf));
		/* one read from two send data, data "111" and "222" of file descriptor */
		n = recv(inv[0], buf, sizeof(buf), 0);
		for (i = 0; i < n; i++)
			fprintf(stdout, "%3d", buf[i]);
#endif
		read_fd(inv[0], ptr, sizeof(ptr), &recv_fd);
		write(recv_fd, "new", sizeof("new") - 1);
		close(inv[0]);
	}
	return 0;
}
