#include <stdio.h>
#include <errno.h>
#include <sys/syslog.h>

#if 0
int main(int argc, char *argv[])
{
	openlog("testsyslog", LOG_CONS | LOG_PID, LOG_USER);
	errno = 1;
	syslog(LOG_DEBUG, "%m\n");
	closelog();
}
#else
int main(int argc, char *argv[])
{
	openlog("testsyslog", LOG_CONS | LOG_PID, LOG_DEBUG);
	syslog(LOG_USER, "syslog test message generated in program %s \n", argv[0]);
	closelog();
	return 0;
}
#endif
