#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int socket_func(int domain, int type, int protocol)
{
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0)
	{
		fprintf(stdout, "socket failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int inet_pton_func(int af, const char *src, void *dst)
{
	int ret;
	if ((ret = inet_pton(af, src, dst)) < 0)
	{
		fprintf(stdout, "inet_pton : address family error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if(ret == 0)
	{
		fprintf(stdout, "inet_pton : address ip format error \n");
		exit(EXIT_FAILURE);
	}
	return ret;
}
int connect_func(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int fd;
	if ((fd = connect(sockfd, addr, addrlen)) < 0)
	{
		fprintf(stdout, "connect error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
pid_t fork_func(void)
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		fprintf(stdout, "fork error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return pid;
}
int select_func(int nfds, fd_set *readfds, fd_set *writefds,
	fd_set *exceptfds, struct timeval *timeout)
{
	int ret;
	if ((ret = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
	{
		fprintf(stdout, "select error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
ssize_t recv_func(int sockfd, void *buf, size_t len, int flags)
{
	int ret;
	if ((ret = recv(sockfd, buf, len, flags)) < 0)
	{
		fprintf(stdout, "recv error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
ssize_t send_func(int sockfd, const void *buf, size_t len, int flags)
{
	int ret;
	if ((ret = send(sockfd, buf, len, flags)) < 0)
	{
		fprintf(stdout, "send error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
ssize_t read_func(int fd, void *buf, size_t count)
{
	int ret;
	if ((ret = read(fd, buf, count)) < 0)
	{
		fprintf(stdout, "read error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
ssize_t write_func(int fd, const void *buf, size_t count)
{
	int ret;
	if ((ret = write(fd, buf, count)) < 0)
	{
		fprintf(stdout, "wrie error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int shutdown_func(int sockfd, int how)
{
	int ret;
	if ((ret = shutdown(sockfd, how)) < 0)
	{
		fprintf(stdout, "shutdown error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int kill_func(pid_t pid, int sig)
{
	int ret;
	if ((ret = kill(pid, sig)) < 0)
	{
		fprintf(stdout, "kill error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return ret;
}
int main(int argc, char *argv[])
{
	int fd;
	fd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	connect_func(fd, (struct sockaddr *)&saddr, sizeof(saddr));

	pid_t pid;
	fd_set rset;
	int n;
	char buf[1024];
	if ((pid = fork_func()) == 0)
	{
		/* child process */
		for ( ; ; )
		{
			FD_ZERO(&rset);
			FD_SET(fd, &rset);
			select_func(fd + 1, &rset, NULL, NULL, NULL);
			pid_t ppid;
			n = recv_func(fd, buf, sizeof(buf), 0);
			if (n == 0)
			{
				ppid = getppid();
				if (ppid != 1)
				{
					close(fd);
					kill_func(ppid, SIGTERM);
					exit(EXIT_SUCCESS);
				}
				else
				{
					close(fd);
					exit(EXIT_FAILURE);
				}
			}
			write_func(STDOUT_FILENO, buf, n);
		}
		
	}
	else if (pid > 0)
	{
		/* parent process */
		for ( ; ; )
		{
			FD_ZERO(&rset);
			FD_SET(STDIN_FILENO, &rset);
			select_func(STDIN_FILENO + 1, &rset, NULL, NULL, NULL);
			bzero(buf, sizeof(buf));
			n = read_func(STDIN_FILENO, buf, sizeof(buf));
			if (n == 0)
			{
				shutdown_func(fd, SHUT_WR);
				pause();
				return 0;
			}
			send_func(fd, buf, n, 0);
		}
	}
	return 0;
}
