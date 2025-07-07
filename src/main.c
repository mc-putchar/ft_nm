#include <stddef.h>
#include <unistd.h>
#include "ft_nm.h"

#ifdef DEBUG
# include "ft_printf.h"
void	debug(const char *msg)
{
	ft_printf("DEBUG: %s\n", msg);
}
#else
void	debug(const char *msg)
{
	(void)msg;
}
#endif

int	main(int ac, char **av)
{
	int	err;

	if (ac < 2)
		err = names(DEFAULT_TARGET);
	else
		err = names(av[1]);
	return (err);
}
