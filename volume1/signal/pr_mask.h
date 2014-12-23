#include <stdio.h>
#include <signal.h>
#include <errno.h>

void pr_mask(char *prompt)
{
	sigset_t set;
	int save_errno;
	save_errno = errno;

	sigprocmask(0, NULL, &set);
	fprintf(stdout ,"%s : ", prompt);
#if 1
	if (sigismember(&set, SIGHUP)) fprintf(stdout, "SIGHUP ");
	if (sigismember(&set, SIGINT)) fprintf(stdout, "SIGINT ");
	if (sigismember(&set, SIGQUIT)) fprintf(stdout, "SIGQUIT "); 	
	if (sigismember(&set, SIGILL)) fprintf(stdout, "SIGILL ");
	if (sigismember(&set, SIGTRAP)) fprintf(stdout, "SIGTRAP "); 	
	if (sigismember(&set, SIGABRT)) fprintf(stdout, "SIGABRT "); 	
	//if (sigismember(&set, SIGEMT)) fprintf(stdout, "SIGEMT ");
	if (sigismember(&set, SIGFPE)) fprintf(stdout, "SIGFPE ");
	if (sigismember(&set, SIGKILL)) fprintf(stdout, "SIGKILL "); 	
	if (sigismember(&set, SIGBUS)) fprintf(stdout, "SIGBUS ");
	if (sigismember(&set, SIGSEGV)) fprintf(stdout, "SIGSEGV "); 	
	if (sigismember(&set, SIGSYS)) fprintf(stdout, "SIGSYS ");
	if (sigismember(&set, SIGPIPE)) fprintf(stdout, "SIGPIPE "); 	
	if (sigismember(&set, SIGALRM)) fprintf(stdout, "SIGALRM "); 	
	if (sigismember(&set, SIGTERM)) fprintf(stdout, "SIGTERM "); 	
	if (sigismember(&set, SIGURG)) fprintf(stdout, "SIGURG ");
	if (sigismember(&set, SIGSTOP)) fprintf(stdout, "SIGSTOP "); 	
	if (sigismember(&set, SIGTSTP)) fprintf(stdout, "SIGTSTP "); 	
	if (sigismember(&set, SIGCONT)) fprintf(stdout, "SIGCONT "); 	
	if (sigismember(&set, SIGCHLD)) fprintf(stdout, "SIGCHLD "); 	
	if (sigismember(&set, SIGTTIN)) fprintf(stdout, "SIGTTIN "); 	
	if (sigismember(&set, SIGTTOU)) fprintf(stdout, "SIGTTOU "); 	
	if (sigismember(&set, SIGIO)) fprintf(stdout, "SIGIO ");
	if (sigismember(&set, SIGXCPU)) fprintf(stdout, "SIGXCPU "); 	
	if (sigismember(&set, SIGXFSZ)) fprintf(stdout, "SIGXFSZ "); 	
	if (sigismember(&set, SIGVTALRM)) fprintf(stdout, "SIGVTALRM ");
	if (sigismember(&set, SIGPROF)) fprintf(stdout, "SIGPROF "); 	
	if (sigismember(&set, SIGWINCH)) fprintf(stdout, "SIGWINCH ");
	//if (sigismember(&set, SIGINFO)) fprintf(stdout, "SIGINFO "); 	
	if (sigismember(&set, SIGUSR1)) fprintf(stdout, "SIGUSR1 "); 	
	if (sigismember(&set, SIGUSR2)) fprintf(stdout, "SIGUSR2 "); 	
	putchar('\n');
#endif
	errno = save_errno ;
	return;
}
