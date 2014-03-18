#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char a[10] = "123456";
	char b[] = "456";
	if (sizeof(a) / sizeof(*b) - 1 - strlen(a) >= strlen(b))
		strcat(a,b);
	else
		printf("does not rich space\n");
	printf("a:%s\n", a);
	return 0;
}
