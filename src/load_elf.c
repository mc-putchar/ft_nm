/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_elf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 19:38:04 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/15 02:54:57 by mcutura          ###   ########.fr       */
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

static int	validate_type(t_elf *elf)
{
	uint16_t	e_type;

	if (elf->is64)
		e_type = load_uint16(elf->u_dat.ehdr->e_type, elf->swap);
	else
		e_type = load_uint16(elf->u_dat.ehdr32->e_type, elf->swap);
	if (e_type != ET_NONE)
		return (0);
	return (ft_dprintf(STDERR_FILENO, ERR_ELF_TYPE, e_type), -1);
}

static int	validate_header(t_elf *elf)
{
	if (ft_memcmp(elf->u_dat.ehdr->e_ident, ELFMAG, SELFMAG))
		return (-1);
	if (elf->u_dat.ehdr->e_ident[EI_DATA] == ELFDATANONE)
		return (-1);
	elf->swap = 0;
	if (elf->u_dat.ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
		elf->swap = !is_lsb();
	else if (elf->u_dat.ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
		elf->swap = is_lsb();
	if (elf->u_dat.ehdr->e_ident[EI_VERSION] != EV_CURRENT \
	|| elf->u_dat.ehdr->e_ident[EI_CLASS] == ELFCLASSNONE)
		return (-1);
	elf->is64 = (elf->u_dat.ehdr->e_ident[EI_CLASS] == ELFCLASS64);
	return (validate_type(elf));
}

static int	load_file_to_mem(t_elf *elf, int fd)
{
	if (!elf || elf->size < sizeof(Elf64_Ehdr))
	{
		ft_dprintf(STDERR_FILENO, ERR_BAD_ELF, elf->filename);
		return (-1);
	}
	elf->u_dat.addr = mmap(NULL, elf->size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (elf->u_dat.addr == MAP_FAILED)
		return (throw_error(-1, ERR_MMAP));
	if (validate_header(elf))
	{
		munmap(elf->u_dat.addr, elf->size);
		ft_dprintf(STDERR_FILENO, ERR_BAD_ELF, elf->filename);
		return (-1);
	}
	return (0);
}

int	load_file(char const *file, t_elf *elf, uint32_t opts)
{
	int const	fd = open(file, O_RDONLY);
	struct stat	statbuf;

	if (fd < 0)
		return (ft_dprintf(STDERR_FILENO, ERR_FILE_OPEN, file), -1);
	statbuf = (struct stat){0};
	elf->filename = (char *)file;
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
