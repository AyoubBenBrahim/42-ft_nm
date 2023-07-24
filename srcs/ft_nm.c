#include "../inc/ft_nm.h"

#define MAX_FILES 10



// Print an error message and exit the program
void error(char *msg)
{
    fprintf(stderr, "Error: %s (%s)\n", msg, strerror(errno));
    exit(1);
}

int open_file(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        ft_printf("open: '%s': can't open file\n", path);
        error("Failed to open");
    }
    return fd;
}

// Map a file into memory and return a pointer to the mapped data
void *map_file(int fd, size_t size)
{
    void *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED)
    {
        error("mmap: Failed to map file");
    }
    return data;
}

// Unmap a mapped file and close its file descriptor
void unmap_file(void *data, size_t size, int fd)
{
    if (munmap(data, size) == -1)
    {
        error("Failed to unmap file");
    }
    if (close(fd) == -1)
    {
        error("Failed to close file");
    }
}

// Parse the ELF header of a file and return a pointer to the ELF header
void parse_ehdr(t_file *file)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)(file->data);
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3)
    {
        error("Invalid ELF header");
    }
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32 && ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
        error("Invalid ELF class/arch");
    }
    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB &&
        ehdr->e_ident[EI_DATA] != ELFDATA2MSB)
    {
        error("Invalid ELF data encoding(LSB/MSB)");
    }
    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN && ehdr->e_type != ET_CORE)
    {
        error("Invalid ELF type");
    }
    if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
    {
        file->is64 = false;
        file->ehdr32 = (Elf32_Ehdr *)(file->data);
        // file->shdr32 = (Elf32_Shdr *)((char *)file->data + file->ehdr32->e_shoff);
        // file->syms32 = (Elf32_Sym *)((char *)file->data + file->shdr32[file->ehdr32->e_shstrndx].sh_offset);
    }
    else
    {
        file->is64 = true;
        file->ehdr64 = (Elf64_Ehdr *)(file->data);
        // file->shdr64 = (Elf64_Shdr *)((char *)file->data + file->ehdr64->e_shoff);
        // file->syms64 = (Elf64_Sym *)((char *)file->data + file->shdr64[file->ehdr64->e_shstrndx].sh_offset);
    }
}

// Parse the section header table of a file and return a pointer to thesection header table
/*
* man elf:
* The ELF header's e_shoff member gives the
* byte offset from the beginning of the file
* to the section header table.
*/
void *parse_shdr(t_file *file)
{
    if (file->is64)
    {
        file->shdr64 = (Elf64_Shdr *)((char *)file->data + file->ehdr64->e_shoff);
        if (file->ehdr64->e_shoff == 0 || file->ehdr64->e_shentsize != sizeof(file->ehdr64) || file->ehdr64->e_shnum == 0)
            error("Invalid section header table");
        
    }
    else
    {
        file->shdr64 = (Elf64_Shdr *)((char *)file->data + file->ehdr64->e_shoff);
        if (file->ehdr64->e_shoff == 0 || file->ehdr64->e_shentsize != sizeof(file->ehdr64) || file->ehdr64->e_shnum == 0)
            error("Invalid section header table");
    }
}

// Parse the symbol table of a file and return a pointer to the symbol table
// Elf_Sym *parse_symtab(void *data, Elf_Shdr *shdr, size_t shnum, size_t strtab_index, size_t *n_syms)
void parse_symtab(t_file *file, size_t strtab_index)
{
    size_t i;
    Elf32_Sym *syms32 = NULL;
    Elf64_Sym *syms64 = NULL;

    Elf32_shdr *STRTAB = NULL;

    for (i = 0; i < file->ehdr32->e_shnum; i++)
    {
        if (file->shdr32[i].sh_type == SHT_SYMTAB)
        {
            if (file->shdr32[i].sh_link != strtab_index)
                error("Invalid symbol table");
            
            syms32 = (Elf32_Sym *)((char *)file->data + file->shdr32[i].sh_offset);
            file->n_syms = file->shdr32[i].sh_size / sizeof(Elf32_Sym);
            break;
        }
    }
    if (syms32 == NULL)
        error("No symbol table found");
    
    file->syms32 = syms32;


  Elf32_Shdr *symtab_section = NULL;
  Elf32_Shdr *strtab_section = NULL;
  for (int i = 0; i < header->e_shnum; ++i) {
    if (sections[i].sh_name > sections[header->e_shstrndx].sh_size)
      return ft_printf("'%s': error bad section header at %d\n", file->path, i), 1;
    if (sections[i].sh_type == SHT_SYMTAB) {
      symtab_section = &sections[i];
      strtab_section = &sections[sections[i].sh_link];
      break;
    }
  }
  if (symtab_section == NULL || strtab_section == NULL)
    return ft_printf("'%s': no symbol table found\n", file->path), 1;


}

void get_data_and_size(t_file *file)
{
    if (fstat(file->fd, &file->stat) == -1)
    {
        ft_printf("fstat: '%s': can't stat file\n", file->path);
        exit(1);
    }
    file->data_size = file->stat.st_size;
    file->data = map_file(file->fd, file->stat.st_size);
}

// Open multiple files and map them into memory
void manageELF(t_file files[], int n_files)
{
    int i;
    for (i = 0; i < n_files; i++)
    {
        files[i].fd = open_file(files[i].path);
        if (fstat(files[i].fd, &files[i].stat) == -1)
        {
            error("Failed to get file size");
        }
        get_data_and_size(&files[i]);
        parse_ehdr(&files[i]);
        parse_shdr(&files[i]);
        // files[i].syms = parse_symtab(files[i].data, files[i].shdr, files[i].ehdr->e_shnum, files[i].shdr[files[i].ehdr->e_shstrndx].sh_link, &files[i].n_syms);
        parse_symtab(&files[i], files[i].shdr32[files[i].ehdr32->e_shstrndx].sh_link);
    }
}

// Close multiple files and unmap them from memory
void close_files(t_file files[], int n_files)
{
    int i;
    for (i = 0; i < n_files; i++)
    {
        unmap_file(files[i].data, files[i].data_size, files[i].fd);
    }
}

// Print the symbol table of a file
void print_symtab(t_file *file)
{
    size_t i;
    const char *strtab = (const char *)file->data + file->shdr[file->ehdr->e_shstrndx].sh_offset;
    printf("Symbol table '%s':\n", file->path);
    printf("%-20s %-10s %-10s %-10s %-5s %-10s\n", "Name", "Value", "Size", "Type", "Bind", "Visibility");
    for (i = 0; i < file->n_syms; i++)
    {
        const char *name = strtab + file->syms[i].st_name;
        printf("%-20s %08lx %08lx ", name, file->syms[i].st_value, file->syms[i].st_size);
        switch (ELF64_ST_TYPE(file->syms[i].st_info))
        {
        case STT_NOTYPE:
            printf("NOTYPE    ");
            break;
        case STT_OBJECT:
            printf("OBJECT    ");
            break;
        case STT_FUNC:
            printf("FUNC      ");
            break;
        case STT_SECTION:
            printf("SECTION   ");
            break;
        case STT_FILE:
            printf("FILE      ");
            break;
        case STT_COMMON:
            printf("COMMON    ");
            break;
        case STT_TLS:
            printf("TLS       ");
            break;
        default:
            printf("UNKNOWN   ");
            break;
        }
        switch (ELF64_ST_BIND(file->syms[i].st_info))
        {
        case STB_LOCAL:
            printf("LOCAL     ");
            break;
        case STB_GLOBAL:
            printf("GLOBAL    ");
            break;
        case STB_WEAK:
            printf("WEAK      ");
            break;
        case STB_GNU_UNIQUE:
            printf("GNU_UNIQUE");
            break;
        default:
            printf("UNKNOWN   ");
            break;
        }
        switch (ELF64_ST_VISIBILITY(file->syms[i].st_other))
        {
        case STV_DEFAULT:
            printf("DEFAULT\n");
            break;
        case STV_INTERNAL:
            printf("INTERNAL\n");
            break;
        case STV_HIDDEN:
            printf("HIDDEN\n");
            break;
        case STV_PROTECTED:
            printf("PROTECTED\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int i;
    t_file files[MAX_FILES];

    // Parse command-line arguments and open the input files
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
        exit(1);
    }
    if (argc - 1 > MAX_FILES)
    {
        fprintf(stderr, "Error: too many input files (maximum is %d)\n", MAX_FILES);
        exit(1);
    }
    ft_memset(files, 0, sizeof(files));
    for (i = 1; i < argc; i++)
    {
        files[i - 1].path = argv[i];
    }
    manageELF(files, argc - 1);

    // Print the symbol tables of the input files
    for (i = 0; i < argc - 1; i++)
    {
        print_symtab(&files[i]);
    }

    // Close the input files
    close_files(files, argc - 1);

    return 0;
}