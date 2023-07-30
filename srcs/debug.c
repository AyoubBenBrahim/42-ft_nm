#include "../inc/ft_nm.h"

void print_header32(t_file *file)
{
    ft_printf("ELF header:\n");

    ft_printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++) {
        ft_printf("%.2x ", file->ehdr32->e_ident[i]);
    }
    ft_printf("\n");

    ft_printf("  Class:                             ");
    switch (file->ehdr32->e_ident[EI_CLASS]) {
        case ELFCLASS32:
            ft_printf("ELF32\n");
            break;

        case ELFCLASS64:
            ft_printf("ELF64\n");
            break;

        default:
            ft_printf("Invalid\n");
            break;
    }

    ft_printf("  Data:                              ");
    switch (file->ehdr32->e_ident[EI_DATA]) {
        case ELFDATA2LSB:
            ft_printf("2's complement, little endian\n");
            break;

        case ELFDATA2MSB:
            ft_printf("2's complement, big endian\n");
            break;

        default:
            ft_printf("Invalid\n");
            break;
    }

    ft_printf("  Version:                           %d\n", file->ehdr32->e_ident[EI_VERSION]);

    ft_printf("  OS/ABI:                            ");
    switch (file->ehdr32->e_ident[EI_OSABI]) {
        case ELFOSABI_SYSV:
            ft_printf("UNIX System V ABI\n");
            break;

        case ELFOSABI_HPUX:
            ft_printf("HP-UX ABI\n");
            break;

        case ELFOSABI_NETBSD:
            ft_printf("NetBSD ABI\n");
            break;

        case ELFOSABI_LINUX:
            ft_printf("Linux ABI\n");
            break;

        case ELFOSABI_SOLARIS:
            ft_printf("Solaris ABI\n");
            break;

        case ELFOSABI_IRIX:
            ft_printf("IRIX ABI\n");
            break;

        case ELFOSABI_FREEBSD:
            ft_printf("FreeBSD ABI\n");
            break;

        case ELFOSABI_TRU64:
            ft_printf("Compaq TRU64 UNIX ABI\n");
            break;

        case ELFOSABI_ARM:
            ft_printf("ARM architecture ABI\n");
            break;

        case ELFOSABI_STANDALONE:
            ft_printf("Standalone (embedded) application\n");
            break;

        default:
            ft_printf("Unknown\n");
            break;
    }

    ft_printf("  Type:                              ");
    switch (file->ehdr32->e_type) {
        case ET_NONE:
            ft_printf("None\n");
            break;

        case ET_REL:
            ft_printf("Relocatable file\n");
            break;

        case ET_EXEC:
            ft_printf("Executable file\n");
            break;

        case ET_DYN:
            ft_printf("Shared object file\n");
            break;

        case ET_CORE:
            ft_printf("Core file\n");
            break;

        default:
            ft_printf("Unknown\n");
            break;
    }

    ft_printf("  Machine:                           ");
    switch (file->ehdr32->e_machine) {
        case EM_NONE:
            ft_printf("None\n");
            break;

        case EM_386:
            ft_printf("Intel 80386\n");
            break;

        case EM_X86_64:
            ft_printf("AMD x86-64 architecture\n");
            break;

        case EM_ARM:
            ft_printf("ARM architecture\n");
            break;

        default:
            ft_printf("Unknown\n");
            break;
    }

    ft_printf("  Version:                           0x%x\n", file->ehdr32->e_version);

    ft_printf("  Entry point address:               0x%x\n", file->ehdr32->e_entry);

    ft_printf("  Start of program headers:          %d (bytes into file)\n", file->ehdr32->e_phoff);

    ft_printf("  Start of section headers:          %d (bytes into file)\n", file->ehdr32->e_shoff);

    ft_printf("  Flags:                             0x%x\n", file->ehdr32->e_flags);

    ft_printf("  Size of this header:               %d (bytes)\n", file->ehdr32->e_ehsize);

    ft_printf("  Size of program headers:            %d (bytes)\n", file->ehdr32->e_phentsize);

    ft_printf("  Number of program headers:         %d\n", file->ehdr32->e_phnum);

    ft_printf("  Size of section headers:            %d (bytes)\n", file->ehdr32->e_shentsize);

    ft_printf("  Number of section headers:         %d\n", file->ehdr32->e_shnum);

    ft_printf("  Section header string table index: %d\n", file->ehdr32->e_shstrndx);
}

/*
**********************************************************************
*/

void print_header64(t_file *file)
{
    ft_printf("ELF header:\n");

    ft_printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++) {
        ft_printf("%.2x ", file->ehdr64->e_ident[i]);
    }
    ft_printf("\n");

    ft_printf("  Class:                             ");
    switch (file->ehdr64->e_ident[EI_CLASS]) {
        case ELFCLASS32:
            ft_printf("ELF32\n");
            break;

        case ELFCLASS64:
            ft_printf("ELF64\n");
            break;

        default:
            ft_printf("Invalid\n");
            break;
    }

    ft_printf("  Data:                              ");
    switch (file->ehdr64->e_ident[EI_DATA]) {
        case ELFDATA2LSB:
            ft_printf("2's complement, little endian\n");
            break;

        case ELFDATA2MSB:
            ft_printf("2's complement, big endian\n");
            break;

        default:
            ft_printf("Invalid\n");
            break;
    }

    ft_printf("  Version:                           %d\n", file->ehdr64->e_ident[EI_VERSION]);

    ft_printf("  OS/ABI:                            ");
    switch (file->ehdr64->e_ident[EI_OSABI]) {
        case ELFOSABI_SYSV:
            ft_printf("UNIX System V ABI\n");
            break;

        case ELFOSABI_HPUX:
            ft_printf("HP-UX ABI\n");
            break;

        case ELFOSABI_NETBSD:
            ft_printf("NetBSD ABI\n");
            break;

        case ELFOSABI_LINUX:
            ft_printf("Linux ABI\n");
            break;

        case ELFOSABI_SOLARIS:
            ft_printf("Solaris ABI\n");
            break;

        case ELFOSABI_IRIX:
            ft_printf("IRIX ABI\n");
            break;

        case ELFOSABI_FREEBSD:
            ft_printf("FreeBSD ABI\n");
            break;

        case ELFOSABI_TRU64:
            ft_printf("Compaq TRU64 UNIX ABI\n");
            break;

        case ELFOSABI_ARM:
            ft_printf("ARM architecture ABI\n");
            break;

        case ELFOSABI_STANDALONE:
            ft_printf("Standalone (embedded) application\n");
            break;

        default: 
            ft_printf("Unknown\n");
            break;
    
    }

    ft_printf("  Type:                              ");
    switch (file->ehdr64->e_type) {
        case ET_NONE:
            ft_printf("None\n");
            break;

        case ET_REL:
            ft_printf("Relocatable file\n");
            break;

        case ET_EXEC:
            ft_printf("Executable file\n");
            break;

        case ET_DYN:
            ft_printf("Shared object file\n");
            break;

        case ET_CORE:
            ft_printf("Core file\n");
            break;

        default:
            ft_printf("Unknown\n");
            break;
    }

    ft_printf("  Machine:                           ");
    switch (file->ehdr64->e_machine) {
        case EM_NONE:
            ft_printf("None\n");
            break;

        case EM_386:
            ft_printf("Intel 80386\n");
            break;

        case EM_X86_64:
            ft_printf("AMD x86-64 architecture\n");
            break;

        case EM_ARM:
            ft_printf("ARM architecture\n");
            break;

        default:
            ft_printf("Unknown\n");
            break;
    }

    ft_printf("  Version:                           0x%x\n", file->ehdr64->e_version);

    ft_printf("  Entry point address:               0x%lx\n", file->ehdr64->e_entry);

    ft_printf("  Start of program headers:          %lu (bytes into file)\n", file->ehdr64->e_phoff);

    ft_printf("  Start of section headers:          %lu (bytes into file)\n", file->ehdr64->e_shoff);

    ft_printf("  Flags:                             0x%x\n", file->ehdr64->e_flags);

    ft_printf("  Size of this header:               %d (bytes)\n", file->ehdr64->e_ehsize);

    ft_printf("  Size of program headers:            %d (bytes)\n", file->ehdr64->e_phentsize);

    ft_printf("  Number of program headers:         %d\n", file->ehdr64->e_phnum);

    ft_printf("  Size of section headers:            %d (bytes)\n", file->ehdr64->e_shentsize);

    ft_printf("  Number of section headers:         %d\n", file->ehdr64->e_shnum);

    ft_printf("  Section header string table index: %d\n", file->ehdr64->e_shstrndx);

}

/*
**********************************************************************
*/

void print_section_headers(t_file *file)
{
    if (file->ehdr32->e_ident[EI_CLASS] == ELFCLASS32)
    {
        for (size_t i = 0; i < file->ehdr32->e_shnum; i++)
        {
            Elf32_Shdr *shdr = &file->shdr32[i];

            ft_printf("Section header #%zu:\n", i);

            // ft_printf("  Name:                              %s\n", &file->strtabOffset[shdr->sh_name]);

            ft_printf("  Type:                              ");
            switch (shdr->sh_type) {
                case SHT_NULL:
                    ft_printf("NULL\n");
                    break;

                case SHT_PROGBITS:
                    ft_printf("Program data\n");
                    break;

                case SHT_SYMTAB:
                    ft_printf("Symbol table\n");
                    break;

                case SHT_STRTAB:
                    ft_printf("String table\n");
                    break;

                case SHT_RELA:
                    ft_printf("Relocation entries with addends\n");
                    break;

                case SHT_HASH:
                    ft_printf("Symbol hash table\n");
                    break;

                case SHT_DYNAMIC:
                    ft_printf("Dynamic linking information\n");
                    break;

                case SHT_NOTE:
                    ft_printf("Notes\n");
                    break;

                case SHT_NOBITS:
                    ft_printf("Program space with no data (bss)\n");
                    break;

                case SHT_REL:
                    ft_printf("Relocation entries, no addends\n");
                    break;

                case SHT_SHLIB:
                    ft_printf("Reserved\n");
                    break;

                case SHT_DYNSYM:
                    ft_printf("Dynamic linker symbol table\n");
                    break;

                default:
                    ft_printf("Unknown\n");
                    break;
            }

            ft_printf("  Address:                           0x%x\n", shdr->sh_addr);

            ft_printf("  Offset:                            %d (bytes into file)\n", shdr->sh_offset);

            ft_printf("  Size:                              %d (bytes)\n", shdr->sh_size);

            ft_printf("  EntSize:                           %d (bytes)\n", shdr->sh_entsize);

            ft_printf("  Flags:                             0x%x\n", shdr->sh_flags);

            ft_printf("  Link:                              %d\n", shdr->sh_link);

            ft_printf("  Info:                              %d\n", shdr->sh_info);

            ft_printf("  Addralign:                         %d (bytes)\n", shdr->sh_addralign);

            ft_printf("  Name table index:                  %d\n", shdr->sh_link);
        }
    }
    else if (file->ehdr32->e_ident[EI_CLASS] == ELFCLASS64)
    {
        for (size_t i = 0; i < file->ehdr64->e_shnum; i++)
        {
            Elf64_Shdr *shdr = &file->shdr64[i];

            ft_printf("Section header #%zu:\n", i);

            // ft_printf("  Name:                              %s\n", &file->strtabOffset[shdr->sh_name]);

            ft_printf("  Type:                              ");
            switch (shdr->sh_type) {
                case SHT_NULL:
                    ft_printf("NULL\n");
                    break;

                case SHT_PROGBITS:
                    ft_printf("Program data\n");
                    break;

                case SHT_SYMTAB:
                    ft_printf("Symbol table\n");
                    break;

                case SHT_STRTAB:
                    ft_printf("String table\n");
                    break;

                case SHT_RELA:
                    ft_printf("Relocation entries with addends\n");
                    break;

                case SHT_HASH:
                    ft_printf("Symbol hash table\n");
                    break;

                case SHT_DYNAMIC:
                    ft_printf("Dynamic linking information\n");
                    break;

                case SHT_NOTE:
                    ft_printf("Notes\n");
                    break;

                case SHT_NOBITS:
                    ft_printf("Program space with no data (bss)\n");
                    break;

                case SHT_REL:
                    ft_printf("Relocation entries, no addends\n");
                    break;

                case SHT_SHLIB:
                    ft_printf("Reserved\n");
                    break;

                case SHT_DYNSYM:
                    ft_printf("Dynamic linker symbol table\n");
                    break;

                default:
                    ft_printf("Unknown\n");
                    break;
            }

            ft_printf("  Address:                           0x%lx\n", shdr->sh_addr);

            ft_printf("  Offset:                            %lu (bytes into file)\n", shdr->sh_offset);

            ft_printf("  Size:                              %lu (bytes)\\n", shdr->sh_size);

            ft_printf("  EntSize:                           %lu (bytes)\n", shdr->sh_entsize);

            ft_printf("  Flags:                             0x%x\n", shdr->sh_flags);

            ft_printf("  Link:                              %d\n", shdr->sh_link);

            ft_printf("  Info:                              %d\n", shdr->sh_info);

            ft_printf("  Addralign:                         %lu (bytes)\n", shdr->sh_addralign);

            ft_printf("  Name table index:                  %d\n", shdr->sh_link);
        }
    }
}

/*
**********************************************************************
*/

void display_symtab(t_file *file)
{
    t_syms *syms = file->syms;
    Elf32_Sym *sym;
    char *name;

    printf("Symbol table '%s' contains %zu entries:\n", file->path, syms->n_syms);
    printf("%6s: %8s %8s %3s %3s %3s %s\n", "Num", "Value", "Size", "Type", "Bind", "Vis", "Name");

    for (size_t i = 0; i < syms->n_syms; i++)
    {
        sym = (Elf32_Sym *)((char *)syms->symtab + i * sizeof(Elf32_Sym));
        name = (char *)syms->strtab + sym->st_name;

        printf("%6zu: %08x %08x %3s %3s %3s %s\n", i, sym->st_value, sym->st_size,
               get_symbol_type(sym->st_info), get_symbol_bind(sym->st_info), get_symbol_visibility(sym->st_other), name);
    }
}

char *get_symbol_type(unsigned char info)
{
    switch (ELF32_ST_TYPE(info))
    {
        case STT_NOTYPE: return "NOTYPE";
        case STT_OBJECT: return "OBJECT";
        case STT_FUNC: return "FUNC";
        case STT_SECTION: return "SECTION";
        case STT_FILE: return "FILE";
        case STT_COMMON: return "COMMON";
        case STT_TLS: return "TLS";
        case STT_NUM: return "NUM";
        case STT_LOOS: return "LOOS";
        case STT_HIOS: return "HIOS";
        case STT_LOPROC: return "LOPROC";
        case STT_HIPROC: return "HIPROC";
        default: return "UNKNOWN";
    }
}

char *get_symbol_bind(unsigned char info)
{
    switch (ELF32_ST_BIND(info))
    {
        case STB_LOCAL: return "LOCAL";
        case STB_GLOBAL: return "GLOBAL";
        case STB_WEAK: return "WEAK";
        case STB_NUM: return "NUM";
        case STB_LOOS: return "LOOS";
        case STB_HIOS: return "HIOS";
        case STB_LOPROC: return "LOPROC";
        case STB_HIPROC: return "HIPROC";
        default: return "UNKNOWN";
    }
}

char *get_symbol_visibility(unsigned char other)
{
    switch (ELF32_ST_VISIBILITY(other))
    {
        case STV_DEFAULT: return "DEFAULT";
        case STV_INTERNAL: return "INTERNAL";
        case STV_HIDDEN: return "HIDDEN";
        case STV_PROTECTED: return "PROTECTED";
        default: return "UNKNOWN";
    }
}