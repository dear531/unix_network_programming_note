#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


int sendfd(int pass, int source)
{
#define CONTEXT_RECV	""
#define SOURCE_FILE		"2.txt"
	struct iovec iov[1];
	iov[0].iov_base = CONTEXT_RECV;
	iov[0].iov_len = sizeof(CONTEXT_RECV);
	union control_un {
		struct cmsghdr cmsg;
		char msg_control[CMSG_SPACE(sizeof(int))];
	} control_un;
	struct msghdr msg;
	msg.msg_iov = iov;
	msg.msg_iovlen = sizeof(iov) / sizeof(*iov);
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_flags = 0;
	msg.msg_control = control_un.msg_control;
	msg.msg_controllen = sizeof(control_un);
	struct cmsghdr *pcmsg;
	pcmsg = CMSG_FIRSTHDR(&msg);	
	pcmsg->cmsg_len = sizeof(control_un);
	pcmsg->cmsg_level = SOL_SOCKET;
	pcmsg->cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(pcmsg) = source;

	return sendmsg(pass, &msg, 0);
}
int recvfd(int pass, int *dest)
{
	union control_un {
		struct cmsghdr cmsg;
		char msg_control[CMSG_SPACE(sizeof(int))];
	} control_un;
#if 0
         struct iovec {
             void *iov_base;   /* Starting address */
             size_t iov_len;   /* Number of bytes */
         };
#endif
	struct iovec iov[1];
	char buff[1024];
	iov[0].iov_base = buff;
	iov[0].iov_len = sizeof(buff);
#if 0
         struct msghdr {
             void         *msg_name;       /* optional address */
             socklen_t     msg_namelen;    /* size of address */
             struct iovec *msg_iov;        /* scatter/gather array */
             size_t        msg_iovlen;     /* # elements in msg_iov */
             void         *msg_control;    /* ancillary data, see below */
             socklen_t     msg_controllen; /* ancillary data buffer len */
             int           msg_flags;      /* flags on received message */
         };
#endif
	struct msghdr msg;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = sizeof(iov) / sizeof(*iov);
	msg.msg_control = control_un.msg_control;
	msg.msg_controllen = sizeof(control_un);
	int ret;
	ret = recvmsg(pass, &msg, 0);
	if (0 > ret) {
		fprintf(stderr, "recvmsg error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
#if 0
       struct cmsghdr {
               socklen_t   cmsg_len;   /* data byte count, including header */
               int         cmsg_level; /* originating protocol */
               int         cmsg_type;  /* protocol-specific type */
       /* followed by  unsigned char   cmsg_data[]; */
       };
#endif
	struct cmsghdr *pcm;
	pcm = CMSG_FIRSTHDR(&msg);
	if (NULL == pcm || sizeof (control_un) != pcm->cmsg_len) {
		goto failed;
	}
	if (SOL_SOCKET != pcm->cmsg_level || SCM_RIGHTS != pcm->cmsg_type) {
		goto failed;
	}
	*dest = *(int *)CMSG_DATA(pcm);
	fprintf(stdout, "*dest :%d\n", *dest);
	return *dest;
failed:
	fprintf(stdout, "normal error\n");
	return -1;
}

int main(int argc, char *argv[])
{
	/* fork */
	pid_t pid;
	/* create socketpair */
	int pass[2];
	int ret;
	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pass);
	if (0 > ret) {
		fprintf(stderr, "socketpair error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (0 > (pid = fork())) {
		/* fork error */
	} else if (0 < pid) {
		int source;
		/* parent */
		/* close socketpair 0 */
		if (0 < pass[0]) {
			close(pass[0]);
			pass[0] = -1;
		}
		/* open file descriptor */
		source = open(SOURCE_FILE, O_CREAT|O_WRONLY, S_IRUSR | S_IWUSR);
		if (0 > source) {
			fprintf(stderr, "open file %s error %s\n",
					SOURCE_FILE, strerror(errno));
			exit(EXIT_FAILURE);
		}
		/* send to child fd */
		sendfd(pass[1], source);
		/* close fd */
		if (0 < source) {
			close(source);
			source = -1;
		}
		/* close socketpair 1 */
		if (0 < pass[1]) {
			close(pass[1]);
			pass[1] = -1;
		}
	} else {
		/* child */
		int dest;
		/* close socketpair 1 */
		if (0 < pass[1]) {
			close(pass[1]);
			pass[1] = -1;
		}
		/* recv file descriptor from parent*/
		recvfd(pass[0], &dest);
		fprintf(stdout, "dest :%d\n", dest);
		/* write fd */
#define COUTEXT_WRITE	"12345"
		int n;
		if (0 > (n = write(dest, COUTEXT_WRITE, sizeof(COUTEXT_WRITE)))) {
			fprintf(stderr, "write error in child :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		} else {
			fprintf(stdout, "write success\n");
		}
		/* close fd */
		if (0 < dest) {
			close(dest);
			dest = -1;
		}
		/* close socketpair 0 */
		if (0 < pass[0]) {
			close(pass[0]);
			pass[0] = -1;
		}
	}
	return 0;
}
