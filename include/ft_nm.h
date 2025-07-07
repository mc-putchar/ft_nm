/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:14:27 by mcutura           #+#    #+#             */
/*   Updated: 2025/07/07 02:55:12 by mcutura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <elf.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stddef.h>
# include <stdint.h>

# ifdef DEBUG
#  define DEBUG 1
# else
#  define DEBUG 0
# endif

# define DEFAULT_TARGET	"a.out"
# define ERR_FILE_OPEN	"Error: Cannot open file '%s'\n"
# define ERR_MMAP		"Error: Failed mapping file to memory\n"
# define ERR_ELF_HEADER	"Error: Not a valid ELF file\n"
# define ERR_ELF_TYPE	"Error: Unsupported ELF type: %d\n"
# define ERR_BAD_ELF	"Error: Bad ELF file\n"
# define ERR_OOB		"Error: Offset %#x is out of bounds for file size %#x\n"
# define ERR_NOT_IMPL	"Error: Feature not implemented\n"

# define ELF_HEADER_FMT	"  %-8s %02x %02x %02x %02x %02x %02x %02x %02x %02x \
%02x %02x %02x %02x %02x %02x %02x\n"

typedef struct s_elf
{
	char		*filename;
	size_t		size;
	int			fd;
	union
	{
		void		*addr;
		Elf64_Ehdr	*ehdr;
		Elf32_Ehdr	*ehdr32;
	}			u_dat;
}	t_elf;

void		debug(const char *msg);
void		print_file_info(char *path, struct stat *st);
void		print_elf_ident(Elf64_Ehdr *ehdr);
void		print_elf_header64(Elf64_Ehdr *ehdr);
void		print_elf_header32(Elf32_Ehdr *ehdr);
void		print_raw_bytes(void *data, size_t size);
void		print_program_headers(t_elf *elf);
void		print_section_headers(t_elf *elf);
void		print_strtab(size_t offset, size_t size, t_elf *elf);
void		print_symbols(t_elf *elf, Elf64_Shdr *shdr, char *strtab, \
				size_t strtab_size);

int			names(char *file);
void		*seek_elf(t_elf *elf, size_t off, size_t len);
char const	*get_section_type_str(uint32_t type);
char		*get_string_table(t_elf *elf, size_t offset, size_t size, \
				size_t *len);

#endif
