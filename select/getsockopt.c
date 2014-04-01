#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
	int fd;
	int optval;
	int optlen = sizeof(optval);
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	fprintf(stdout, "socket received buf size :%d\n", optval);
	optval = 10000;
	setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optval, optlen);
	getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	fprintf(stdout, "socket received buf size :%d\n", optval);

	getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
	fprintf(stdout, "socket send buf size :%d\n", optval);

	getsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen);
	fprintf(stdout, "socket keep-alive time :%d\n", optval);

	struct linger lg;
	int lglen = sizeof(lg);
	getsockopt(fd, SOL_SOCKET, SO_LINGER, &lg, &lglen);
	fprintf(stdout, "socket linger time onoff:%d, linger%d\n", lg.l_onoff, lg.l_linger);

	int udpfd;
	udpfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpfd < 0)
		fprintf(stdout, "create udp socket failed\n");
	getsockopt(udpfd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
	fprintf(stdout, "socket udp send buf size :%d\n", optval);
	

	return 0;
}
