/* -----mqcreate1.c-------- */
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP)
int main(int argc, char **argv)
{
        int c, flags;
        mqd_t mqd;
 
        flags = O_RDWR | O_CREAT;
 
        while ((c = getopt(argc, argv, "e")) != -1)
        {
                switch (c)
                {
                        case 'e':       flags |= O_EXCL;
                        break;
                }
        }
        if (optind != argc -1)
        {
                perror("usage:mqcreate [-e] <name>");
                exit(1);
        }
 
        mqd = mq_open(argv[optind], flags, FILE_MODE, NULL );
		fprintf(stdout, "argv:%s\n",
				argv[optind]);
        if (mqd == -1)
        {
                perror("mq_open error");
        }
        printf("mqd = %d\n", mqd);
        mq_close(mqd);
        exit(0);
}
