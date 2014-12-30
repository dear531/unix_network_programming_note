#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


#if 0
          struct stat {
              dev_t     st_dev;     /* ID of device containing file */
              ino_t     st_ino;     /* inode number */
              mode_t    st_mode;    /* protection */
              nlink_t   st_nlink;   /* number of hard links */
              uid_t     st_uid;     /* user ID of owner */
              gid_t     st_gid;     /* group ID of owner */
              dev_t     st_rdev;    /* device ID (if special file) */
              off_t     st_size;    /* total size, in bytes */
              blksize_t st_blksize; /* blocksize for filesystem I/O */
              blkcnt_t  st_blocks;  /* number of blocks allocated */
              time_t    st_atime;   /* time of last access */
              time_t    st_mtime;   /* time of last modification */
              time_t    st_ctime;   /* time of last status change */
          };
#endif
int main(void)
{
	struct stat stat;
	int fd;
	fd = open("1.txt", O_CREAT | O_EXCL);
	if (0 > fd) {
		fprintf(stdout, "open erorr :%s\n",
				strerror(errno));
	}
	fstat(fd, &stat);
	fprintf(stdout, "stat.st_dev:%x, stat.st_ino:%x, ftok:%x\n",
			stat.st_dev, stat.st_ino, ftok("1.txt", 0xff));
	if (0 < fd)
		close(fd);
	return 0;
}
