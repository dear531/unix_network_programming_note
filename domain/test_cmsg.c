#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{	
	fprintf(stdout, "CMSG_ALIGN:%ld\n", (long)CMSG_ALIGN(sizeof(int)));
	fprintf(stdout, "CMSG_SPACE:%ld\n", (long)CMSG_SPACE(sizeof(int)));	
	fprintf(stdout, "CMSG_LEN:%ld\n", (long)CMSG_LEN(sizeof(int)));
	fprintf(stdout, "sizeof cmsg :%ld\n", (long)sizeof(struct cmsghdr));
#if 0
#define CMSG_ALIGN(len) ( ((len)+sizeof(long)-1) & ~(sizeof(long)-1) )
#define CMSG_SPACE(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
#define CMSG_LEN(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))
#endif
	return 0;
}
