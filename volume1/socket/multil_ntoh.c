#include <stdio.h>

#include "bit_check.h"

int main(int argc, char *argv[])
{
	int i = 0x01020304;
	int tmp_i;
	short j = 0x0102;
	short tmp_j;
	bit_check(&i, sizeof(i));
	tmp_i = htonl(i);
	bit_check(&tmp_i, sizeof(tmp_i));

	bit_check(&j, sizeof(j));
	tmp_j = htons(j);
	bit_check(&tmp_j, sizeof(tmp_j));

	return 0;
}
