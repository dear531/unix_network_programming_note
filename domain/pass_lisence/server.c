#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "head.h"

int read_lisence(int fd, void *ptr, int nbytes, struct msghdr *msg)
{
	int n;
	struct iovec iov[1];
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg->msg_name = NULL;
	msg->msg_namelen = 0;
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	struct ucred *plsc;
	union
	{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(*plsc))];
	}control_lisence;
	msg->msg_control = control_lisence.control;
	msg->msg_controllen = sizeof(control_lisence.control);
	n = recvmsg(fd, msg, 0);
	struct cmsghdr *pcmsg;
	pcmsg = CMSG_FIRSTHDR(msg);
	if (
	pcmsg->cmsg_len != CMSG_LEN(sizeof(*plsc)) &&
	pcmsg->cmsg_level != SOL_SOCKET &&
	pcmsg->cmsg_type != SCM_RIGHTS)
		goto end;
	plsc = (struct ucred *)CMSG_DATA(pcmsg);
	fprintf(stdout, "pid:%d, uid:%d, gid:%d", plsc->pid, plsc->uid, plsc->gid);
end:
	return n;
}
int main(int argc, char *argv[])
{
	/* socket of unix domain protocol */
	int lifd;
	if ((lifd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stdout, "create socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* bind fo domain */
	unlink(UNXI_PATH);
	struct sockaddr_un unaddr;
	bzero(&unaddr, sizeof(unaddr));
	unaddr.sun_family = AF_LOCAL;
	bcopy(UNXI_PATH, &unaddr.sun_path, min(strlen(UNXI_PATH), sizeof(unaddr.sun_path) - 1));
	if (bind(lifd, (struct sockaddr *)&unaddr, sizeof(unaddr)) < 0)
	{
		fprintf(stdout, "bind error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* listen */
	if (listen(lifd, 10) < 0)
	{
		fprintf(stdout, "listen error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* accept */
	int cofd;
	if ((cofd = accept(lifd, NULL, NULL)) < 0)
	{
		fprintf(stdout, "accpet error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	char buf[1024];
	bzero(buf, sizeof(buf));
	struct msghdr msg;
	read_lisence(cofd, buf, sizeof(buf), &msg);
#if 0
	recv(cofd, buf, sizeof(buf), 0);
	send(cofd, "222", sizeof("222"), 0);
#endif
	unlink(UNXI_PATH);
	/* close connect */
	if (cofd != -1)
		close(cofd);
	/* close socket */
	close(lifd);
	return 0;
}
