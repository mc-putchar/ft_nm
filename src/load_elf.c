/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_elf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 19:38:04 by mcutura           #+#    #+#             */
/*   Updated: 2025/08/10 21:50:36 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "ft_nm.h"
#include "ft_printf.h"
#include "libft_mem.h"

void	*seek_elf(t_elf *elf, size_t off, size_t len)
{
	if (off + len > elf->size)
	{
		if (DEBUG)
			ft_dprintf(STDERR_FILENO, ERR_OOB, off + len, elf->size);
		return (NULL);
	}
	return ((char *)elf->u_dat.addr + off);
}

int	validate_header32(Elf32_Ehdr *ehdr)
{
	if (ehdr->e_type == ET_NONE)
		return (ft_dprintf(STDERR_FILENO, ERR_ELF_TYPE, ehdr->e_type), -1);
	return (0);
}

int	validate_header64(Elf64_Ehdr *ehdr)
{
	if (ehdr->e_type == ET_NONE)
		return (ft_dprintf(STDERR_FILENO, ERR_ELF_TYPE, ehdr->e_type), -1);
	return (0);
}

int	validate_header(t_elf *elf)
{
	if (ft_memcmp(elf->u_dat.ehdr->e_ident, ELFMAG, SELFMAG))
		return (throw_error(-1, ERR_ELF_HEADER));
	if (elf->u_dat.ehdr->e_ident[EI_DATA] == EV_NONE)
		return (throw_error(-1, ERR_BAD_ELF));
	if (elf->u_dat.ehdr->e_ident[EI_VERSION] != EV_CURRENT)
		return (throw_error(-1, ERR_BAD_ELF));
	if (elf->u_dat.ehdr->e_ident[EI_CLASS] == ELFCLASS32)
		return (throw_error(-1, ERR_NOT_IMPL));
	else if (elf->u_dat.ehdr->e_ident[EI_CLASS] == ELFCLASS64)
		return (validate_header64(elf->u_dat.ehdr));
	return (throw_error(-1, ERR_BAD_ELF));
}

int	load_file_to_mem(t_elf *elf, int fd)
{
	if (!elf || elf->size < sizeof(Elf64_Ehdr))
		return (throw_error(-1, ERR_BAD_ELF));
	elf->u_dat.addr = mmap(NULL, elf->size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (elf->u_dat.addr == MAP_FAILED)
		return (throw_error(-1, ERR_MMAP));
	if (validate_header(elf))
	{
		munmap(elf->u_dat.addr, elf->size);
		return (throw_error(-1, ERR_BAD_ELF));
	}
	return (0);
}

int	load_file(char *file, t_elf *elf, uint32_t opts)
{
	int const	fd = open(file, O_RDONLY);
	struct stat	statbuf;

	if (fd < 0)
		return (ft_dprintf(STDERR_FILENO, ERR_FILE_OPEN, file), -1);
	statbuf = (struct stat){0};
	elf->filename = file;
	if (fstat(fd, &statbuf))
		return (close(fd), throw_error(-1, ERR_FSTAT));
	elf->size = (size_t)statbuf.st_size;
	if (load_file_to_mem(elf, fd))
		return (close(fd), -1);
	(void)close(fd);
	if (opts & OPT_FILENAME)
		ft_printf("\n%s:\n", file);
	return (0);
}
