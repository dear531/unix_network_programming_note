#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int argc, char *argv[])
{
	if (2 > argc) {
		fprintf(stderr, "usage :./a.out msgid\n");
		exit(EXIT_FAILURE);
	}
	int ret;
	ret = msgctl(strtol(argv[1], NULL, 10), IPC_RMID, NULL);
	if (0 < ret) {
		fprintf(stderr, "msgctl IPC_RMID error :%s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}
