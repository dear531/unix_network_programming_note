#include <unistd.h>
int main(int argc, char *argv[])
{
	close(STDIN_FILENO);
	for ( ; ; )
	{
		write(STDOUT_FILENO, ".", sizeof("."));
		sleep(1);
	}
	return 0;
}
