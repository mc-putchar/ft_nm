#include <stddef.h>
#include <unistd.h>
#include "ft_nm.h"

int	main(int ac, char **av)
{
	int	err;

	if (ac < 2)
		err = names(DEFAULT_TARGET);
	else
		err = names(av[1]);
	return (err);
}
