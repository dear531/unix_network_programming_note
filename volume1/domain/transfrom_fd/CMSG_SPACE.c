#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	fprintf(stdout, "CMSG_SPACE(sizeof(int)) :%ld\n", (long)CMSG_SPACE(sizeof(int)));
	fprintf(stdout, "CMSG_SPACE(sizeof(char)) :%ld\n", (long)CMSG_SPACE(sizeof(char)));
	
	return 0;
}
