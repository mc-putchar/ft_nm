#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ft_nm.h"
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

static char const	*endian_str(int endian)
{
	if (endian == ELFDATA2LSB)
		return ("little endian");
	else if (endian == ELFDATA2MSB)
		return ("big endian");
	return ("none (invalid)");
}

static char const	*class_str(int class)
{
	if (class == ELFCLASS32)
		return ("ELF32");
	else if (class == ELFCLASS64)
		return ("ELF64");
	return ("unknown");
}

static char const	*type_str(int type)
{
	if (type == ET_NONE)
		return ("None");
	else if (type == ET_REL)
		return ("Relocatable");
	else if (type == ET_EXEC)
		return ("Executable");
	else if (type == ET_DYN)
		return ("Shared object");
	else if (type == ET_CORE)
		return ("Core file");
	return ("Unknown type");
}

static char const	*machine_str(int machine)
{
	if (machine == EM_386)
		return ("i386");
	else if (machine == EM_X86_64)
		return ("amd64");
	else if (machine == EM_ARM)
		return ("arm");
	else if (machine == EM_AARCH64)
		return ("aarch64");
	return ("unknown");
}

void	print_elf_ident(Elf64_Ehdr *ehdr)
{
	ft_printf("ELF Header:\n");
	ft_printf(ELF_HEADER_FMT, "Magic:",
		ehdr->e_ident[EI_MAG0], ehdr->e_ident[EI_MAG1],
		ehdr->e_ident[EI_MAG2], ehdr->e_ident[EI_MAG3],
		ehdr->e_ident[EI_CLASS], ehdr->e_ident[EI_DATA],
		ehdr->e_ident[EI_VERSION], ehdr->e_ident[EI_OSABI],
		ehdr->e_ident[EI_ABIVERSION], ehdr->e_ident[EI_PAD],
		ehdr->e_ident[EI_PAD + 1], ehdr->e_ident[EI_PAD + 2],
		ehdr->e_ident[EI_PAD + 3], ehdr->e_ident[EI_PAD + 4],
		ehdr->e_ident[EI_PAD + 5], ehdr->e_ident[EI_PAD + 6]);
	ft_printf("  %-34s %s\n", "Class:", class_str(ehdr->e_ident[EI_CLASS]));
	ft_printf("  %-34s %s\n", "Data:", endian_str(ehdr->e_ident[EI_DATA]));
	ft_printf("  %-34s %d\n", "Version:", ehdr->e_ident[EI_VERSION]);
	ft_printf("  %-34s %d\n", "OS/ABI:", ehdr->e_ident[EI_OSABI]);
	ft_printf("  %-34s %d\n", "ABI Version:", ehdr->e_ident[EI_ABIVERSION]);
}

void	print_elf_header32(Elf32_Ehdr *ehdr)
{
	ft_printf("  %-34s %d (%s)\n", "Type:", ehdr->e_type, type_str(ehdr->e_type));
	ft_printf("  %-34s %d (%s)\n", "Machine:", ehdr->e_machine, machine_str(ehdr->e_machine));
	ft_printf("  %-34s %d\n", "Version:", ehdr->e_version);
	ft_printf("  %-34s %#.8x\n", "Entry point address:", ehdr->e_entry);
	ft_printf("  %-34s %d (bytes into file)\n", "Start of program headers:", ehdr->e_phoff);
	ft_printf("  %-34s %d (bytes into file)\n", "Start of section headers:", ehdr->e_shoff);
	ft_printf("  %-34s %#x\n", "Flags:", ehdr->e_flags);
	ft_printf("  %-34s %d (bytes)\n", "Size of this header:", ehdr->e_ehsize);
	ft_printf("  %-34s %d (bytes)\n", "Size of program headers:", ehdr->e_phentsize);
	ft_printf("  %-34s %d\n", "Number of program headers:", ehdr->e_phnum);
	ft_printf("  %-34s %d (bytes)\n", "Size of section headers:", ehdr->e_shentsize);
	ft_printf("  %-34s %d\n", "Number of section headers:", ehdr->e_shnum);
	ft_printf("  %-34s %d\n", "Section header string table index:", ehdr->e_shstrndx);
}

#include <stdio.h>
void	print_elf_header64(Elf64_Ehdr *ehdr)
{
	ft_printf("  %-34s %d (%s)\n", "Type:", ehdr->e_type, type_str(ehdr->e_type));
	ft_printf("  %-34s %#X (%s)\n", "Machine:", ehdr->e_machine, machine_str(ehdr->e_machine));
	ft_printf("  %-34s %d\n", "Version:", ehdr->e_version);
	ft_printf("  %-34s %#.8x\n", "Entry point address:", ehdr->e_entry);
	printf("  %-34s %ld (bytes into file)\n", "Start of program headers:", ehdr->e_phoff);
	printf("  %-34s %ld (bytes into file)\n", "Start of section headers:", ehdr->e_shoff);
	ft_printf("  %-34s %#x\n", "Flags:", ehdr->e_flags);
	ft_printf("  %-34s %d (bytes)\n", "Size of this header:", ehdr->e_ehsize);
	ft_printf("  %-34s %d (bytes)\n", "Size of program headers:", ehdr->e_phentsize);
	ft_printf("  %-34s %d\n", "Number of program headers:", ehdr->e_phnum);
	ft_printf("  %-34s %d (bytes)\n", "Size of section headers:", ehdr->e_shentsize);
	ft_printf("  %-34s %d\n", "Number of section headers:", ehdr->e_shnum);
	ft_printf("  %-34s %d\n", "Section header string table index:", ehdr->e_shstrndx);
}

void	print_raw_bytes(void *data, size_t size)
{
	unsigned char	*bytes = (unsigned char *)data;
	size_t			i;

	ft_printf("Raw bytes:\n");
	i = 0;
	while (i < sizeof(Elf64_Ehdr) && i < size)
	{
		if (!(i & 15))
			ft_printf("\n%10.8x: ", i);
		else if (!(i & 7))
			ft_printf("  ");
		ft_printf("%02x ", bytes[i]);
		i++;
	}
	ft_printf("\n");
}

void	print_program_header(Elf64_Phdr *phdr, uint32_t idx)
{
	ft_printf("Program Header: [%u]\n", idx);
	ft_printf("  %-34s %d\n", "Type:", phdr->p_type);
	ft_printf("  %-34s %#.8x\n", "Flags:", phdr->p_flags);
	ft_printf("  %-34s %#.8x\n", "Offset:", phdr->p_offset);
	ft_printf("  %-34s %#.8x\n", "Virtual address:", phdr->p_vaddr);
	ft_printf("  %-34s %#.8x\n", "Physical address:", phdr->p_paddr);
	ft_printf("  %-34s %d (bytes)\n", "File size:", phdr->p_filesz);
	ft_printf("  %-34s %d (bytes)\n", "Memory size:", phdr->p_memsz);
	ft_printf("  %-34s %d (bytes)\n", "Alignment:", phdr->p_align);
}

void	print_program_headers(t_elf *elf)
{
	Elf64_Ehdr const	*ehdr = elf->u_dat.ehdr;
	Elf64_Phdr			*phdr;
	size_t				i;
	size_t				offset;

	ft_printf("Program Headers (%d entries):\n", ehdr->e_phnum);
	if (ehdr->e_phnum == 0)
	{
		ft_printf("  No program headers.\n");
		return ;
	}
	i = 0;
	while (i < ehdr->e_phnum)
	{
		offset = ehdr->e_phoff + i * ehdr->e_phentsize;
		phdr = (Elf64_Phdr *)seek_elf(elf, offset, ehdr->e_phentsize);
		if (!phdr)
			return ;
		print_program_header(phdr, (uint32_t)i);
		++i;
	}
}
