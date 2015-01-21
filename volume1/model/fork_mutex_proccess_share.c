#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 


pthread_mutexattr_t mutexattr;
pthread_mutex_t *pmutex;

void my_lock_init(void)
{
	int fd;
	fd = open("/dev/zero", O_RDWR);
	if (0 > fd) {
		fprintf(stderr, "open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
#if 0
       void *mmap(void *start, size_t length, int prot, int flags,
                  int fd, off_t offset);

#endif
	pmutex = mmap(NULL, sizeof(*pmutex), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (MAP_FAILED == pmutex) {
		fprintf(stderr, "mmap error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (0 < fd) {
		close(fd);
		fd = -1;
	}
	int ret;
	ret = pthread_mutexattr_init(&mutexattr);
	if (0 != ret) {
		fprintf(stderr, "pthread_mutexattr_init error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	ret = pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
	if (0 != ret) {
		fprintf(stderr, "pthread_mutexattr_setpshared error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	ret = pthread_mutex_init(pmutex, &mutexattr);
	if (0 != ret) {
		fprintf(stderr, "pthread_mutex_init error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return;
}
int create_listen(void)
{
	int fd;
	/* socket */
	fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (0 > fd) {
		fprintf(stderr, "socket error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* bind */
#if 0
         struct sockaddr_in {
             sa_family_t    sin_family; /* address family: AF_INET */
             u_int16_t      sin_port;   /* port in network byte order */
             struct in_addr sin_addr;   /* internet address */
         };
#endif
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(8000);
	int ret;
	ret = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	if (0 > ret) {
		/* inet_pton error */
		fprintf(stderr, "inet_pton error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	} else if (0 == ret) {
		/* address wrong in current family */
		fprintf(stdout, "address wrong in current family\n");
		exit(EXIT_FAILURE);
	} else {
		/* success */
	}
	bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	/* listen */
	ret = listen(fd, 10);
	if (0 > ret) {
		fprintf(stderr, "listen error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}
int main(int argc, char *argv[])
{
	my_lock_init();
	int fd;
	fd = create_listen();
	pid_t pid;
	if (SIG_ERR == signal(SIGCHLD, SIG_IGN)) {
		fprintf(stderr, "signal error\n");
	}
	int i;
	for (i = 0; 5 > i; i++) {
		if (0 > (pid = fork())) {
			/* fork error */
			fprintf(stderr, "fork error :%s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		} else if (0 < pid) {
			/* parent */
			fprintf(stdout, "parent fork proccess %d\n", pid);
		} else {
			/* child */
			int confd;
			int ret;
			char buff[1024] = {0};
			/* lock */
			fprintf(stdout, "proccess %d\n", getpid());
			ret = pthread_mutex_lock(pmutex);
			if (0 != ret) {
				fprintf(stdout, "mutex_lock error:%s\n",
						strerror(errno));
			}
			fprintf(stdout, "lock mutex proccess %d\n",
					getpid());
			/* accept */
			confd = accept(fd, NULL, NULL);
			ret = pthread_mutex_unlock(pmutex);
			if (0 != ret) {
				fprintf(stdout, "mutex_unlock error:%s\n",
						strerror(errno));
			}
			fprintf(stdout, "unlock mutex proccess %d\n",
					getpid());
			/* release lock */
			if (0 > confd) {
				fprintf(stderr, "accept error :%s\n",
						strerror(errno));
				exit(EXIT_FAILURE);
			}
			int n;
			for ( ; memset(buff, 0x00, sizeof(buff)); ) {
				n = recv(confd, buff, sizeof(buff), 0);
				if (0 > n) {
					/* recv error */
				} else if (0 == n) {
					/* peer close */
				} else {
					fprintf(stdout, "buff:%s\n", buff);
				}
			}
			exit(EXIT_SUCCESS);
		}
	}
	for ( ; ; )
		pause();
	return 0;
}
