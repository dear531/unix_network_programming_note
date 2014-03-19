#include <setjmp.h>
#include <stdio.h>
jmp_buf j;
void raise_exception(void)
{
	printf("exception raised\n");
	longjmp(j, 3); /* jump to exception handler case 3 */
	printf("this line should never appear\n");
}
int main(void)
{
	switch (setjmp(j))
	{
		case 0:
			printf("''setjmp'' is initializing ''j''\n");
			raise_exception();
			printf("this line should never appear\n");
		case 1:
			printf("Case 1\n");break;
		case 2:
			printf("Case 2\n");break;
		case 3:
			printf("Case 3\n");break;
		default:
			break;
	}
	return 0;
}
