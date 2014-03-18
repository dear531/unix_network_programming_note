static inline void bit_check(void *num, int mod)
{
	int i;
	for (mod--; mod >= 0; mod--)
	{
		for (i = 7; i >= 0; i--)
			printf("%d", ((char *)num)[mod] & (1 << i) ? 1 : 0);
		if (mod)
			putchar(',');
	}
	putchar('\n');
}
