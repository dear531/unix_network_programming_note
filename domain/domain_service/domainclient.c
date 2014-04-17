#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>


int main(int argc, char *arg[])
{
	int cofd;
	if ((cofd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stdout, "socket error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	int ret;
	struct sockaddr_un usaddr;
	usaddr.sun_family = AF_LOCAL;
	bcopy("/tmp/unix_tmp", usaddr.sun_path, sizeof("/tmp/unix_tmp"));
	if ((ret = connect(cofd, (struct sockaddr*)&usaddr, sizeof(usaddr))) < 0)
	{
		fprintf(stdout, "connect error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	char sbuf[1024], rbuf[1024];
	int i;
	for (i = 0; i < 10; i++)
	{
		bzero(sbuf, sizeof(sbuf));
		sbuf[0] = '0' + i;
		send(cofd, sbuf, sizeof(sbuf), 0);
		fprintf(stdout, "sbuf :%s\n", sbuf);
		bzero(rbuf, sizeof(rbuf));
tryagain:
		if ((ret = recv(cofd, rbuf, sizeof(sbuf), MSG_DONTWAIT)) > 0)
		{
			fprintf(stdout, "rbuf :%s\n", sbuf);
		}
		else if (ret == 0)
		{
			fprintf(stdout, "peer close \n");
			close(cofd);
			cofd = -1;
			exit(EXIT_FAILURE);
		}
		else if (errno == EAGAIN)
		{
			fprintf(stdout, "try again\n");
			sleep(1);
			goto tryagain;
		}
		else
		{
			fprintf(stdout, "received error :%s\n", strerror(errno));
			close(cofd);
			cofd = -1;
			exit(EXIT_FAILURE);
			
		}
			
		fprintf(stdout, "rbuf :%s\n", rbuf);
	}
	return 0;
}
