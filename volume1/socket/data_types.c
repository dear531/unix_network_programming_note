#include <stdio.h>
#include <sys/types.h>

int main(int agc, char *argv[])
{
	int8_t i8 = 0;
	int16_t i16 = 0;
	printf("sizeof(i8):%ld\n", sizeof(i8));
	printf("sizeof(i16):%ld\n", sizeof(i16));
	return 0;
}
