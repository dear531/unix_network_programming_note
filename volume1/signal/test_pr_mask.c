#include <stdio.h>
#include "pr_mask.h"

int main(int argc, char *argv[])
{
	sigset_t set;
	sigfillset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);
	pr_mask("test");
	return 0;
}
