#include <stdio.h>
#include <setjmp.h>

jmp_buf j;
int func_jmp(void)
{
	fprintf(stdout, "is func_jmp before long_jpm\n");
	longjmp(j,2);
	fprintf(stdout, "is func_jmp before long_jpm, just never app\n");
	return -1;
}
int main(int argc, char *argv[])
{
	int ret;
	if ((ret  = setjmp(j)) == 0)
	{
		/* usuly execute */
		fprintf(stdout, "ret:%d\n", ret);
		fprintf(stdout, "I am main lien before func_jmp\n");
		ret = func_jmp();
		fprintf(stdout, "I'm is func_jmp result ret:%d\n", ret);
		fprintf(stdout, "I am main lien after func_jmp, just never app\n");
	}
	else
	{
		/* longjmp return */
		fprintf(stdout, "ret:%d\n", ret);
		fprintf(stdout, "I'm main line longjmp return nonzero\n");
	}
	return 0;
}
