#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>


int sendfd(int pass, int source)
{
#define CONTEXT_NOTITY	""
#define SOURCE_FILE		"2.txt"
	struct iovec iov[1];
	iov[0].iov_base = CONTEXT_NOTITY;
	iov[0].iov_len = sizeof(CONTEXT_NOTITY);
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
#define CONNECT_NUM	100
int create_listen(void)
{
	int sockfd;
	/* socket */
#if 0
	int socket(int domain, int type, int protocol);
#endif
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (0 > sockfd) {
		fprintf(stderr, "socket error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* set addr ip reuse */
	int opt = 1;
	int ret;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (0 > ret) {
		fprintf(stderr, "setsockopt error :%s\n",
				strerror(errno));
	}
	/* bind */
#if 0
	int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
         struct sockaddr_in {
             sa_family_t    sin_family; /* address family: AF_INET */
             u_int16_t      sin_port;   /* port in network byte order */
             struct in_addr sin_addr;   /* internet address */
         };
#endif
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;	
	addr.sin_port = htons(8000);
#if 0
	int inet_pton(int af, const char *src, void *dst);
#endif
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if (0 > ret) {
		fprintf(stderr, "bind error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* sockfd */
	ret = listen(sockfd, CONNECT_NUM);
	if (0 > ret) {
		fprintf(stderr, "sockfd error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return sockfd;
}
int main(int argc, char *argv[])
{
	/* create listen */
	int listen;
	listen = create_listen();
	/* fork */
	pid_t pid;
	/* create socketpair */
	struct {
		int pass[2];
		int status;
	} cptr[CONNECT_NUM];
	int ret;
	int i;
	for (i = 0; CONNECT_NUM	> i; i++) {
		ret = socketpair(AF_UNIX, SOCK_STREAM, 0, cptr[i].pass);
		if (0 > ret) {
			fprintf(stderr, "socketpair error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
		cptr[i].status = 0;
		if (0 > (pid = fork())) {
			/* fork error */
		} else if (0 < pid) {
			/* parent */
			/* close socketpair 0 */
			if (0 < cptr[i].pass[0]) {
				close(cptr[i].pass[0]);
				cptr[i].pass[0] = -1;
			}
#if 0
			/* open file descriptor */
			source = open(SOURCE_FILE, O_CREAT|O_WRONLY, S_IRUSR | S_IWUSR);
			if (0 > source) {
				fprintf(stderr, "open file %s error %s\n",
						SOURCE_FILE, strerror(errno));
				exit(EXIT_FAILURE);
			}
#else
#endif
		} else {
			/* child */
			/* close listen */
			if (0 < listen) {
				close(listen);
				listen = -1;
			}
			int dest;
			/* close socketpair 1 */
			int j;
			for (j = 0; i >= j; j++) {
				if (0 < cptr[j].pass[1]) {
#if 0
					sleep(1);
					fprintf(stdout, "getpid :%d, i:%d, close pass:%d\n",
							getpid(), i, cptr[j].pass[1]);
#endif
					close(cptr[j].pass[1]);
					cptr[j].pass[1] = -1;
				}

				/* other flower case is 0 is close in parent */
			}
			for ( ; ; ) {
					/* recv file descriptor from parent*/
					recvfd(cptr[i].pass[0], &dest);
					fprintf(stdout, "dest :%d\n", dest);
					/* write fd */
					int n;
					char buff[1024];
					for ( ; memset(buff, 0x00, sizeof(buff)); ) {
						n = recv(dest, buff, sizeof(buff), 0);
						if (0 > n) {
							/* recv error */
							fprintf(stderr, "recv error :%s\n",
									strerror(errno));
						} else if (0 == n) {
							/* peer close */
							fprintf(stdout, "pid :%d peer close\n",
									getpid());
							break;
						} else {
							fprintf(stdout, "process id :%d buff :%s\n",
									getpid(), buff);
						}
						send(dest, buff, strlen(buff), 0);
					}
					/* tell parent ready we're again */
					send(cptr[i].pass[0], CONTEXT_NOTITY, sizeof(CONTEXT_NOTITY), 0);
					/* close fd */
					if (0 < dest) {
						close(dest);
						dest = -1;
					}
			}
			/* close socketpair 0 */
			if (0 < cptr[i].pass[0]) {
				close(cptr[i].pass[0]);
				cptr[i].pass[0] = -1;
			}
			exit(EXIT_SUCCESS);
		}
	}
	/* send to child fd */
	int source;
	fd_set rset;
	int n;
	int fdmax = listen + 1, fdnum = 1;
	int recvn;
	char buff[512];
	for ( ; ; ) {
		/* select */
		FD_ZERO(&rset);
		FD_SET(listen, &rset);
		for (i = 0; CONNECT_NUM > i; i++) {
			if (1 == cptr[i].status) {
				FD_SET(cptr[i].pass[1], &rset);
			}
		}
#if 0
       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
#endif
		n = select(fdmax, &rset, NULL, NULL, NULL);
		fprintf(stdout, "n :%d\n", n);
		if (0 > n) {
			fprintf(stdout, "select error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
		if (FD_ISSET(listen, &rset)) {
			/* accept */
			source  = accept(listen, NULL, NULL);
			if (0 > source) {
				fprintf(stderr, "accept error :%s\n",
						strerror(errno));
			}
			for (i = 0; CONNECT_NUM > i; i++) {
				if (0 == cptr[i].status) {
					sendfd(cptr[i].pass[1], source);
					cptr[i].status = 1;
					fdnum++;
					if (cptr[i].pass[1] + 1 > fdmax) {
						fdmax = cptr[i].pass[1] + 1;
					}
					break;
				}
			}
			/* close fd */
			if (0 < source) {
				close(source);
				source = -1;
			}
			n--;
		}
		for (i = 0; CONNECT_NUM > i; i++) {
			if (1 == cptr[i].status && FD_ISSET(cptr[i].pass[1], &rset)) {
				recv(cptr[i].pass[1], buff, sizeof(buff), 0);
				cptr[i].status = 0;
				fdnum--;
				if (cptr[i].pass[1] + 1 == fdmax) {
					int j, flag = 0;
					for (j = i; 0 <= j; j--) {
						if (1 == cptr[j].status) {
							fdmax = cptr[j].pass[1] + 1;
							flag = 1;
							break;
						}
					}
					if (0 == flag)
						fdmax = listen + 1;
				}
				n--;
				if (0 == n)
					break;
			}
		}
		fprintf(stdout, "fdmax:%d, fdnum:%d\n",
				fdmax, fdnum);
	}
	for (i = 0; CONNECT_NUM > i; i++) {
		/* close socketpair 1 */
		if (0 < cptr[i].pass[1]) {
			close(cptr[i].pass[1]);
			cptr[i].pass[1] = -1;
		}
	}
	return 0;
}
