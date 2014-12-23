#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char buf[10];
	int a, b, c;
	int n;
	memset(buf, 0x00, sizeof(buf));
	n = scanf("%s", buf);
	fprintf(stdout, "n:%d, buf:%s\n", n, buf);
	n = sscanf(buf, "%d,%d,%d", &a, &b, &c);
	fprintf(stdout, "n:%d, a:%d,b:%d,c:%d\n", n, a, b, c);
	return 0;
}
