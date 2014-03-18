#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char b[] = "12345678";
	char *q = NULL;
	q = &b[2];
	printf("b:%s,q:%s\n", b, q);
	bcopy(b, q, sizeof(b) / sizeof(*b));
	printf("b -> q\nb:%s,q:%s\n", b, q);

	char a[] = "12345678";
	char *p = NULL;
	p = &a[2];
	printf("a:%s,p:%s\n", a, p);
	memcpy(p, a, sizeof(a) / sizeof(*a));
	printf("a -> p\na:%s,p:%s\n", a, p);

	return 0;
}
