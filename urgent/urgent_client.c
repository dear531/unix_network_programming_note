#include <stdio.h> 

#include "share.h"
int main(void)
{
	/* create socket */
	int fd;
	fd = socket_func(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* connect */
	struct sockaddr_in saddr;
	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(8000);
	inet_pton_func(AF_INET, "127.0.0.1", &saddr.sin_addr);
	connect_func(fd, (struct sockaddr *)&saddr, sizeof(saddr));
	/* send data */
	send_func(fd, "1", sizeof("1") - 1, 0);
	/* send urgent */
	send_func(fd, "u", sizeof("u") - 1, MSG_OOB);
	//sleep(1);
	/* send urgent urgent multi bytes */
	send_func(fd, "abc", sizeof("abc") - 1, MSG_OOB);
	/* apeend normal data */
	send_func(fd, "def", sizeof("def") - 1, 0);
	/* close */
	close(fd);
	return (0);
}
