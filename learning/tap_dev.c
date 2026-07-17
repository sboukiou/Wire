
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>
#include <stdbool.h>

#define CLEAR(x) memset(&x, 0, sizeof(x))
#define THROW(X) printf("Error: %s\n", X)
#define LOG(X) printf("Info: %s\n", X)
#define BUFF_SIZE 1024

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

int main(void) {

	char	dev_name[IFNAMSIZ]= "wire0";
	int		fd = tun_alloc(dev_name);
	bool	reading = true;

	if (fd < 0) {
		THROW("tun_alloc(): Failed to create the Virtual interface");
		return (1);
	}
	printf("TAP device [%s] is created wiht fd [%d]", dev_name, fd);
	printf("To turn it up run: > sudo ip link set %s up\n", dev_name);
	LOG("Reading packets");

	while (reading) {
		char	buffer[BUFF_SIZE];
		int n = read(fd, buffer, BUFF_SIZE);
		if (n < 0) {
			LOG("main(): Cant to read further packets");
			reading = false;
		}
		printf("Packets read: [%d]\n", n);
		for (int i = 0; i < (n < 16 ? n: 16); i += 1)
			printf("%02x", buffer[i]);
		printf("\n\n");
	}

	close(fd);

	return (0);
}
