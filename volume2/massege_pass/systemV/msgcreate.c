#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>


#define FTOK_FILE "1.txt"

int main(int argc, char *argv[])
{
	int oflog = IPC_CREAT;
	char ch;
	while (-1 != (ch = getopt(argc, argv, "e"))) {
		switch (ch) {
			case 'e':
				oflog |= IPC_EXCL;
				fprintf(stdout, "-e\n");
				break;
		}
	}
	int fd;
	fd = -1;
	fd = open(FTOK_FILE, O_RDWR | O_CREAT);
	if (0 < fd) {
		close(fd);
		fd = -1;
	}
	key_t key;
	key = ftok(FTOK_FILE, 0xff);
	fprintf(stdout, "key :%x\n", key);
	if (-1 == key) {
		fprintf(stdout, "ftok error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	int msgid;
	msgid = msgget(key, oflog);
	if (0 > msgid && EEXIST == errno) {
		fprintf(stdout, "msgid :%d is exists, pleas continue opration\n",
				msgid);
	} else if (0 > msgid) {
		fprintf(stderr, "msgget error :%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "create msgid :%d success\n", msgid);
	}

	return 0;
}
