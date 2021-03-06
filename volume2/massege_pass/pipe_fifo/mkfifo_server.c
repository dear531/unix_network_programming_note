#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

#define PIPEFILE "1.fifo"

int main(int argc, char *argv[])
{
	int ret;
	int fd;
	void (*save_sig)(int);
	if (SIG_ERR == (save_sig = signal(SIGPIPE, SIG_IGN))) {
		fprintf(stderr, "signal set sigpipe failed %s\n",
				strerror(errno));
	}
again:
	ret = mkfifo(PIPEFILE, 755);
	if (0 > ret) {
		fprintf(stderr, "mkfifo error:%s\n",
				strerror(errno));
		unlink(PIPEFILE);
		goto again;
	}
	fd = open(PIPEFILE, O_WRONLY);
	if (0 > fd) {
		fprintf(stdout, "open error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
#if 0
	char buff[sizeof("this is server") + 1] = "this is server";
#else
	char buff[1024] = {0};
	struct {
		int len;
		char *content;
	} data;
	data.content = "this struct pass server";
	data.len = strlen(data.content);
#endif
	int n, i;
	for (i = 0; ;i++) {
#if 0
		n = write(fd, buff, sizeof(buff));
#else
		memset(buff, 0x00, sizeof(buff));
		memcpy(buff, &data.len, sizeof(data.len));
		memcpy(buff + sizeof(data.len), data.content, strlen(data.content));
		n = write(fd, buff, sizeof(data.len) + strlen(data.content) + 1);
#endif
		fprintf(stdout, "n:%d\n", n);
		if (0 > n) {
			/* write error */
			if (EPIPE != errno) {
				exit(EXIT_SUCCESS);
			}
			fprintf(stderr, "client shutdown\n");
			sleep(1);
		} else if (0 == n) {
			/* can not write */
		} else {
			/* write success */
			fprintf(stdout, "buff:%s\n",
					buff);
#if 0
			buff[sizeof("this is server") - 1] = '0' + i;
#else
#endif
			sleep(1);
		}
	}
	if (-1 != fd) {
		close(fd);
		fd = -1;
	}
	unlink(PIPEFILE);
	return 0;
}
