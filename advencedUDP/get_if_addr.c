#include <stdio.h>

#include "share.h"

int main(int argc, char *argv[])
{
	/* create socket udp */	
	int fd;
	fd = socket_func(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//setsockopt();
	/* bind host */
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port	= htons(8000);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	bind_func(fd, (struct sockaddr *)&local, sizeof(local));
	/* tells socket get IP_PKTINFO */
#ifdef IP_PKTINFO
	int opt = 1;
	setsockopt(fd, IPPROTO_IP, IP_PKTINFO, &opt, sizeof(opt));
#endif
	/* create auxiliary data */
	struct iovec	iov[1];
	char buf[1024];
	bzero(buf, sizeof(buf));
	iov[0].iov_base	= buf;
	iov[0].iov_len	= sizeof(buf);
	union
	{
		struct cmsghdr cmsg;
		char control[CMSG_SPACE(sizeof(struct in_pktinfo))];
	}control_un;
	struct msghdr	msg;
	msg.msg_name	= NULL;
	msg.msg_namelen	= 0;
	msg.msg_iov	= iov;
	msg.msg_iovlen	= sizeof(iov) / sizeof(*iov);
	msg.msg_control	= control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	fprintf(stdout, "msg.msg_controllen:%d\n", msg.msg_controllen);
	msg.msg_flags	= IP_PKTINFO;
#if 0
	struct iovec {                    /* Scatter/gather array items */
		void  *iov_base;              /* Starting address */
		size_t iov_len;               /* Number of bytes to transfer */
	};

	struct msghdr {
		void         *msg_name;       /* optional address */
		socklen_t     msg_namelen;    /* size of address */
		struct iovec *msg_iov;        /* scatter/gather array */
		size_t        msg_iovlen;     /* # elements in msg_iov */
		void         *msg_control;    /* ancillary data, see below */
		size_t        msg_controllen; /* ancillary data buffer len */
		int           msg_flags;      /* flags on received message */
	};


#endif
	/* recvmsg of auxiliary data */
	recvmsg(fd, &msg, 0);
	/* get interface from returned */
	fprintf(stdout, "msg.msg_iov[0].iov_base :%s\n", (char *)msg.msg_iov[0].iov_base);
	/* get address from returned */
	int flags;
	flags = msg.msg_flags;
	if (flags & MSG_TRUNC)
	{
		fprintf(stdout, "msg_trunc\n");
		return (-1);
	}
	if (msg.msg_controllen < sizeof(struct cmsghdr))
	{
		fprintf(stdout, "msg.msg_controllen:%d\n", msg.msg_controllen);
		fprintf(stdout, "sizeof(struct cmsghdr):%ld\n", (long)sizeof(struct cmsghdr));
		fprintf(stdout, "len too short\n");
		return (-1);
	}
#if 0
	struct cmsghdr {
		socklen_t cmsg_len;    /* data byte count, including header */
		int       cmsg_level;  /* originating protocol */
		int       cmsg_type;   /* protocol-specific type */
		/* followed by unsigned char cmsg_data[]; */
	};
#endif
	struct in_pktinfo *ppk;
	struct cmsghdr *pcmsg;
	char ip[(128 / 16) * (4 + 1)];
	int i;
	for (pcmsg = CMSG_FIRSTHDR(&msg); pcmsg != NULL; pcmsg = CMSG_NXTHDR(&msg, pcmsg))	
	{
		if (pcmsg->cmsg_level == IPPROTO_IP
			&& pcmsg->cmsg_type == IP_PKTINFO)
		{
			bzero(ip, sizeof(ip));
			ppk = (struct in_pktinfo *)CMSG_DATA(pcmsg);
			fprintf(stdout, "1\n");
			fprintf(stdout, "interface\t:%u\ndestip\t:%s\n",
					ppk->ipi_ifindex,
					inet_ntop(AF_INET, &ppk->ipi_spec_dst, ip, sizeof(ip)));
			bzero(ip, sizeof(ip));
			fprintf(stdout, "hw\t:%s\n",
					inet_ntop(AF_INET, &ppk->ipi_addr, ip, sizeof(ip)));
			for(i = 0; i < 6; i++)
			{
				if (i == 0)
					fprintf(stdout, "hw\t:");
				fprintf(stdout, "%02x", *(((char *)&ppk->ipi_addr) + i));
				if (i == 5)
					fprintf(stdout, "\n");
				else
					fprintf(stdout, ":");
			}
		}
	}
#if 0
	struct in_pktinfo {
		unsigned int   ipi_ifindex;  /* Interface index */
		struct in_addr ipi_spec_dst; /* Local address */
		struct in_addr ipi_addr;     /* Header Destination
						address */
	};
#endif
	return 0;
}
