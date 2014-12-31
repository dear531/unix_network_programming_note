#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>



#define UNIX_PATH "/tmp/unix.sock"
int main(void)
{
	int sockfd, confd;
	sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un sock_un;
	char buff[1024] = {0};
	memset(&sock_un, 0x00, sizeof(sock_un));
	sock_un.sun_family = AF_UNIX;
	memcpy(sock_un.sun_path, UNIX_PATH, sizeof(UNIX_PATH));
	unlink(UNIX_PATH);
	bind(sockfd, (const struct sockaddr *)&sock_un, sizeof(sock_un));
	listen(sockfd, 10);
	confd = accept(sockfd, NULL, NULL);
	int n;
	for ( ; ; memset(buff, 0x00, sizeof(buff))) {
		n = read(confd, buff, sizeof(buff));
		if (0 > n) {
			fprintf(stderr, "read error :%s\n",
					strerror(errno));
			exit(EXIT_SUCCESS);
		} else if (0 == n) {
			close(confd);
			break;
		} else {
			fprintf(stdout, "buff:%s\n", buff);
		}
	}
	unlink(UNIX_PATH);

#if 0
         struct sockaddr_un {
             sa_family_t    sun_family;               /* AF_UNIX */
             char           sun_path[UNIX_PATH_MAX];  /* pathname */
         };
#endif

	
	return 0;
}
