#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

int write_fd(int fd, void *prt, int nbytes, int sendfd)
{
	struct msghdr msg;
	struct iovec iov[1];
	iov[0].iov_base = prt;
	iov[0].iov_len = nbytes;
	union
	{
	struct cmsghdr cm;
	char control[CMSG_SPACE(sizeof(int))];
	}control_un;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = sizeof(iov) / sizeof(*iov);
	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	struct cmsghdr * csmgp;
	csmgp = CMSG_FIRSTHDR(&msg);
	csmgp->cmsg_len = CMSG_LEN(sizeof(int));
	csmgp->cmsg_level = SOL_SOCKET;
	csmgp->cmsg_type = SCM_RIGHTS;
	*((int *)CMSG_DATA(csmgp)) = sendfd;
	/* maybe same number file discrptor with received fd function */
	fprintf(stdout, "*((int *)CMSG_DATA(csmgp)) :%d\n", *((int *)CMSG_DATA(csmgp)));
	return (sendmsg(fd, &msg, 0));
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stdout, "usage :a.out <file descriptor>\n");
		exit(EXIT_FAILURE);
	}
	int ifd;
	ifd = strtol(argv[1], NULL, 10);

	char *file = "1.txt";
	int fd;
	if ((fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRWXU)) < 0)
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
#define FAIL_STATUS	0
#if FAIL_STATUS
	/*
	 * other send opration effect right send and received,
	 * so don't received and send excapt data for file discriptor
	 */
	send(ifd, "111", sizeof("111"), 0);
#endif
	write_fd(ifd, "222", sizeof("222"), fd);

	close(fd);
	return 0;
}
