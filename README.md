# ft_nm

> [!WARNING]  
> WIP  

Educational reimplementation of nm for Linux  

Lists symbols from ELF object files, shared objects, and executables  

---

## Build

Requires: Linux environment and a C toolchain

```bash
make
```

## Usage

```bash
./ft_nm [option(s)] [file(s)]
```

List symbols in [file(s)] (a.out by default).

### Options

* `-a`: Display debugger-only symbols.
* `-g`: Display only external symbols.
* `-u`: Display only undefined symbols.
* `-r`: Reverse the sense of the sort.
* `-p`: Do not sort the symbols.
* `-h`: Display help message.
* `--`: Stop interpreting flags (useful if filename begins with `-`)

---

## Remaining TODOs:

[ ] Endianness transformations  
[ ] ELF32 support  
[ ] Improve symbol type assertions  
[ ] Exclude symbols from default (no options) print  
[ ] Fix sorting  
[ ] Testing suite  
[ ] Print DYNSYM  
