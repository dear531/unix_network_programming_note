#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	char buf[1024];
	bzero(buf, sizeof(buf));
	read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stdout, "buf:%s\n", buf);
	int i;
	for (i = 0; i < 64; i++)
		close(i);
	int ret;
	ret = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stdout, "buf:%s\n", buf);

	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stdout, "buf:%s\n", buf);

	return 0;
}
