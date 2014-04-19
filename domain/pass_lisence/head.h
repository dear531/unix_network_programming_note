#ifndef __HEAD_H__
#define __HEAD_H__
static inline int min(int m, int n)
{
	return ((m > n) ? n : m);
}
struct ucred { 
        unsigned int	pid;
        unsigned int	uid;
        unsigned int	gid;
};
#define UNXI_PATH	"/tmp/unix"
#endif
