#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>



#include "head.h"

int write_lisence(int fd, void *ptr, int nbytes)
{
	struct msghdr dmsg;
	struct msghdr *msg;
	msg = &dmsg;
	struct iovec iov[1];
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg->msg_name = NULL;
	msg->msg_namelen = 0;
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	union
	{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	}control_lisence;
	msg->msg_control = control_lisence.control;
	msg->msg_controllen = sizeof(control_lisence);
	struct cmsghdr *pcmsg;
	struct ucred *plsc;
	pcmsg = CMSG_FIRSTHDR(msg);
	pcmsg->cmsg_len = CMSG_LEN(sizeof(*plsc));
	pcmsg->cmsg_level = SOL_SOCKET;
	pcmsg->cmsg_type = SCM_RIGHTS;
	plsc = (struct ucred *)CMSG_DATA(pcmsg);
	plsc->pid = getpid();
	plsc->uid = geteuid();
	plsc->gid = getegid();
	return (sendmsg(fd, msg, 0));
}
int main(int argc, char *argv[])
{
	/* create socket for unix domain protocol */
	int cofd;
	if ((cofd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* connect */
	struct sockaddr_un unaddr;
	unaddr.sun_family = AF_LOCAL;
	bzero(unaddr.sun_path, sizeof(unaddr.sun_path));
	bcopy(UNXI_PATH	,&unaddr.sun_path, min(strlen(UNXI_PATH), sizeof(unaddr.sun_path) - 1));
	fprintf(stdout, "unaddr.sun_path: %s\n", unaddr.sun_path);
	if (connect(cofd, (struct sockaddr *)&unaddr, sizeof(unaddr)) < 0)
	{
		fprintf(stdout, "connect error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	char buf[1024];
	bzero(buf, sizeof(buf));
#if 0
	send(cofd, "111", sizeof("111"), 0);
	recv(cofd, buf, sizeof(buf), 0);
	fprintf(stdout, "buf :%s\n", buf);
#endif
	write_lisence(cofd, "111", sizeof("111"));
	/* close connect fd */
	/* close socket file discrptor */
	return 0;
}
