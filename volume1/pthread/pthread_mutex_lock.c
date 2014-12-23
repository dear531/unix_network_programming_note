#include <stdio.h>
#include <pthread.h>

pthread_mutex_t pmt;
int main(int argc, char *argv[])
{
	pthread_mutex_lock(&pmt);
	pmt++;
	pthread_mutex_unlock(&pmt);
	return 0;
}
