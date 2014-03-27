#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	fprintf(stdout, "init not assign fd:%d\n", fd);
	fd = open("1.txt", O_RDWR);
	fprintf(stdout, "open file assign fd:%d\n", fd);
	close(fd);
	fprintf(stdout, "ready close fd:%d\n", fd);
	fd = -1;
	fprintf(stdout, "assign \"-1\" fd:%d\n", fd);
	
	return 0;
}
