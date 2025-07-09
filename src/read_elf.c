/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_elf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 19:38:04 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/08 21:12:10 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <fcntl.h>
#include <stddef.h>
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
		return (ft_dprintf(STDERR_FILENO, ERR_OOB, off + len, elf->size), NULL);
	return ((char *)elf->u_dat.addr + off);
}

int	validate_header32(Elf32_Ehdr *ehdr)
{
	if (ehdr->e_type == ET_NONE)
		return (ft_dprintf(STDERR_FILENO, ERR_ELF_TYPE, ehdr->e_type), -1);
	if (DEBUG)
		print_elf_header32(ehdr);
	return (0);
}

int	validate_header64(Elf64_Ehdr *ehdr)
{
	if (ehdr->e_type == ET_NONE)
		return (ft_dprintf(STDERR_FILENO, ERR_ELF_TYPE, ehdr->e_type), -1);
	if (DEBUG)
		print_elf_header64(ehdr);
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
	if (DEBUG)
		print_elf_ident(elf->u_dat.ehdr);
	if (elf->u_dat.ehdr->e_ident[EI_CLASS] == ELFCLASS32)
		return (throw_error(-1, ERR_NOT_IMPL));
		// return (validate_header32(elf->u_dat.ehdr32));
	else if (elf->u_dat.ehdr->e_ident[EI_CLASS] == ELFCLASS64)
		return (validate_header64(elf->u_dat.ehdr));
	return (throw_error(-1, ERR_BAD_ELF));
}

int	load_file_to_mem(t_elf *elf)
{
	if (!elf || elf->fd < 0 || elf->size < sizeof(Elf64_Ehdr))
		return (throw_error(-1, ERR_BAD_ELF));
	elf->u_dat.addr = mmap(NULL, elf->size, PROT_READ, MAP_PRIVATE, elf->fd, 0);
	if (elf->u_dat.addr == MAP_FAILED)
		return (throw_error(-1, ERR_MMAP));
	if (validate_header(elf))
	{
		munmap(elf->u_dat.addr, elf->size);
		return (throw_error(-1, ERR_BAD_ELF));
	}
	if (DEBUG)
	{
		print_program_headers(elf);
		print_section_headers(elf);
	}
	return (0);
}

int	load_file(char *file, t_elf *elf)
{
	int const	fd = open(file, O_RDONLY);
	struct stat	statbuf;

	if (fd < 0)
		return (ft_dprintf(STDERR_FILENO, ERR_FILE_OPEN, file), -1);
	statbuf = (struct stat){0};
	elf->filename = file;
	if (fstat(fd, &statbuf))
		return (close(fd), -1);
	elf->size = (size_t)statbuf.st_size;
	elf->fd = fd;
	if (DEBUG)
		print_file_info(file, &statbuf);
	if (load_file_to_mem(elf))
		return (close(fd), -1);
	(void)close(fd);
	return (0);
}

int	names(char *file)
{
	t_elf	elf;

	elf = (t_elf){0};
	if (!file || !*file)
		file = DEFAULT_TARGET;
	if (load_file(file, &elf))
		return (-1);
	(void)read_section_headers(&elf);
	munmap(elf.u_dat.addr, elf.size);
	return (0);
}
