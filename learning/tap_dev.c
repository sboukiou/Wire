
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>

int tun_alloc(char *dev) {
	struct ifreq ifr;
	int fd, err;

	if ((fd = open("/dev/net/tap", O_RDWR)) < 0) {
		perror("Could not open the /dev/net/tap file\n");
		exit(1);
	}
	CLEAR(ifr);
	ifr.ifr_flags = IFF_UP | IFF_NO_PI;

	if ( *dev )
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	if ( (err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0 ) {
		printf("ERR: Could not ioctl tun: %s\n", strerror(errno));
		close(fd);
		return (err);
	}
	strcpy(dev, ifr.ifr_name);
	return (fd);
}
