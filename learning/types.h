#ifndef TYPES_H
# define TYPES_H

#include <cstdint>

typedef struct s_eth {
	unsigned char dmac[6];
	unsigned char smac[6];
	uint16_t ether_type;
	unsigned char	payload[];

}	t_eth;

#endif /* TYPES_H */
