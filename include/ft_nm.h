/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcutura <mcutura@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:14:27 by mcutura           #+#    #+#             */
/*   Updated: 2025/11/15 14:06:56 by mcutura          ###   ########.fr       */
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
# define ERR_NO_SYM		"ft_nm: %s: no symbols\n"
# define ERR_UNKOWN_OPT	"ft_nm: unknown option '%c'\n"
# define ERR_FILE_OPEN	"ft_nm: cannot open file '%s'\n"
# define ERR_MMAP		"ft_nm: failed mapping file to memory\n"
# define ERR_ELF_TYPE	"ft_nm: unsupported ELF type: %d\n"
# define ERR_BAD_ELF	"ft_nm: %s: file format not recognized\n"
# define ERR_OOB		"ft_nm: offset %#x is out of bounds for file size %#x\n"
# define ERR_NOT_IMPL	"ft_nm: feature not implemented\n"
# define ERR_FSTAT		"ft_nm: failed to get file status\n"
# define ERR_MALLOC		"ft_nm: failed to allocate memory\n"

# define OPT_DBG_SYMS	0x01
# define OPT_DYNAMIC	0x02
# define OPT_EXTERNALS	0x04
# define OPT_UNDEFINED	0x08
# define OPT_REVERSE	0x10
# define OPT_NO_SORT	0x20
# define OPT_HELP		0x40
# define OPT_FILENAME	0x80
# define OPT_32BIT		0x100

# define SYM_IS_DBG		0x01
# define SYM_IS_EXT		0x02
# define SYM_IS_UNDEF	0x04
# define SYM_IS_RDONLY	0x08

typedef struct s_input
{
	int			flagstop;
	uint32_t	opts;
	size_t		file_count;
	char		**files;
}	t_input;

typedef struct s_elf
{
	char		*filename;
	size_t		size;
	union
	{
		void		*addr;
		Elf64_Ehdr	*ehdr;
		Elf32_Ehdr	*ehdr32;
	}			u_dat;
	int			swap;
	int			is64;
}	t_elf;

typedef struct s_section
{
	union
	{
		Elf64_Shdr	*e64;
		Elf32_Shdr	*e32;
	}			u_shdr;
	char		*name;
	void		*data;
	size_t		size;
	uint32_t	type;
}	t_section;

typedef struct s_symbol
{
	union
	{
		Elf64_Sym const	*e64;
		Elf32_Sym const	*e32;
	}				u_entry;
	char			*name;
	size_t			value;
	uint8_t			type;
	uint8_t			flags;
}	t_symbol;

typedef struct s_symbol_count
{
	size_t	symtab;
	size_t	dynsym;
}	t_symbol_count;

typedef struct s_symbols
{
	t_symbol		*symtab;
	t_symbol		*dynsym;
	t_symbol_count	count;
}	t_symbols;

int			parse_input(int ac, char **av, t_input *input);
int			is_lsb(void);
uint16_t	load_uint16(uint16_t val, int swap);
uint32_t	load_uint32(uint32_t val, int swap);
uint64_t	load_uint64(uint64_t val, int swap);
int			names(char const *file, uint32_t opts);
int			load_file(char const *file, t_elf *elf);
void		*seek_elf(t_elf *elf, size_t off, size_t len);
Elf64_Shdr	*get_section_header(t_elf *elf, size_t idx);
Elf32_Shdr	*get_section_header32(t_elf *elf, size_t idx);
int			read_section_headers(t_elf *elf, t_section *sections, \
			struct s_symbol_count *symcount);
int			read_section_headers32(t_elf *elf, t_section *sections, \
			struct s_symbol_count *symcount);
char const	*get_section_type_str(uint32_t type);
char		*get_string_table(t_elf *elf, size_t offset, size_t size, \
				size_t *len);
void		*get_section(t_elf *elf, size_t idx, size_t *len);
void		*get_section32(t_elf *elf, uint32_t idx, uint32_t *len);
uint32_t	get_section_type(t_elf *elf, size_t idx);
uint32_t	get_section_type32(t_elf *elf, uint32_t idx);
uint64_t	get_section_flags(t_elf *elf, size_t idx);
uint32_t	get_section_flags32(t_elf *elf, uint32_t idx);
char		*get_section_name(t_elf *elf, size_t idx);
char		*get_section_name32(t_elf *elf, size_t idx);
int			load_all_symbols(t_elf *elf, t_section *sections, \
			t_symbol *symtab, t_symbol *dynsym);
int			load_all_symbols32(t_elf *elf, t_section *sections, \
			t_symbol *symtab, t_symbol *dynsym);
int			get_symbol_type(t_elf *elf, Elf64_Sym const *sym);
int			get_symbol_type32(t_elf *elf, Elf32_Sym const *sym);
void		print_symbols(char const *file, t_symbol *syms, size_t count, \
			uint32_t opts);

#endif
