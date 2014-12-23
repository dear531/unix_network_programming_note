#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	char buf[] = "1234";
	write(STDOUT_FILENO, buf, strlen(buf));
	putchar('\n');
	return 0;
}
