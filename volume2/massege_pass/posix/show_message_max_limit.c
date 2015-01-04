#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>


int main(int argc, char *argv[])
{
#if 0
            struct rlimit {
                rlim_t rlim_cur;  /* Soft limit */
                rlim_t rlim_max;  /* Hard limit (ceiling for rlim_cur) */
            };
#endif
	struct rlimit mqlimit;
	getrlimit(RLIMIT_MSGQUEUE, &mqlimit);
	fprintf(stdout, "soft limit:%d, hard limit:%d\n",
			mqlimit.rlim_cur, mqlimit.rlim_max);
	return 0;
}
