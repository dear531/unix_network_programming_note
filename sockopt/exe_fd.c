#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd;
	if (argc < 2)
	{
		fprintf(stdout, "please appropriately argment\n");
		exit(EXIT_FAILURE);
	}
	
	fd = strtol(argv[1], NULL, 10);
	char buf[] = "1234567890";
	int n;
	n = write(fd, buf, sizeof(buf));
	if (n < 0)
	{
		fprintf(stdout, "execute write fd failed :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "here execute write fd :%d\n", fd);
	fsync(fd);
	close(fd);

	return 0;
}
