#include <unistd.h>
#include "ft_nm.h"

#define DEFAULT_TARGET	"a.out"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ft_printf.h"

#define ERR_FILE_OPEN	"Error: Cannot open file '%s'\n"
int	get_file_info(char *filepath)
{
	int const	fd = open(filepath, O_RDONLY);
	struct stat	statbuf;

	if (fd < 0)
		return (ft_dprintf(STDERR_FILENO, ERR_FILE_OPEN, filepath), -1);
	statbuf = (struct stat){0};
	if (fstat(fd, &statbuf))
		return (close(fd), -1);
	print_file_info(filepath, &statbuf);
	(void)close(fd);
	return (0);
}

int	main(int ac, char **av)
{
	int	err;

	if (ac < 2)
		err = get_file_info(DEFAULT_TARGET);
	else
		err = get_file_info(av[1]);
	return (err);
}
