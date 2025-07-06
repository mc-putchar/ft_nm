#include <sys/types.h>
#include <sys/stat.h>
#include "ft_printf.h"

void	print_file_info(char *path, struct stat *st)
{
	ft_printf("\nFile information:\n");
	ft_printf("%-32s %16s\n", "File:", path);
	ft_printf("%-32s %16d\n", "Device ID:", st->st_dev);
	ft_printf("%-32s %16d\n", "Inode number:", st->st_ino);
	ft_printf("%-32s %16d\n", "File mode:", st->st_mode);
	ft_printf("%-32s %16d\n", "Number of links:", st->st_nlink);
	ft_printf("%-32s %16d\n", "User ID:", st->st_uid);
	ft_printf("%-32s %16d\n", "Group ID:", st->st_gid);
	ft_printf("%-32s %16d\n", "Device ID (if special file):", st->st_rdev);
	ft_printf("%-32s %16d\n", "File size (bytes):", st->st_size);
	ft_printf("%-32s %16d\n", "Block size for filesystem I/O:", st->st_blksize);
	ft_printf("%-32s %16d\n", "Number of blocks allocated:", st->st_blocks);
}
