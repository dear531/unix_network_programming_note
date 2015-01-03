#include <stdio.h>

int main(int argc, char *argv[])
{
	mq_unlink("/temp.1234");	
	return 0;
}
