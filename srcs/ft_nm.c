#include "../inc/ft_nm.h"

#define MAX_FILES 10

void error(const char *msg)
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

void unmap_file(void *data, size_t size, int fd)
{
    if (munmap(data, size) == -1)
        error("Failed to unmap file");

    if (close(fd) == -1)
        error("Failed to close file");
}

// Parse the ELF header of a file and save a pointer to the ELF header
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
        error("Invalid ELF class/arch");

    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB && ehdr->e_ident[EI_DATA] != ELFDATA2MSB)
        error("Invalid ELF data encoding(LSB/MSB)");

    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN && ehdr->e_type != ET_CORE)
        error("Invalid ELF type");

    if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
    {
        file->is64 = false;
        file->ehdr32 = (Elf32_Ehdr *)(file->data);
    }
    else
    {
        file->is64 = true;
        file->ehdr64 = (Elf64_Ehdr *)(file->data);
    }
}

// Parse the section header table of a file and save a pointer to the section header table
/*
 * man elf:
 * The ELF header's e_shoff member gives the
 * byte offset from the beginning of the file
 * to the section header table.
 */
void parse_shdr(t_file *file)
{
    if (file->is64)
    {
        file->shdr64 = (Elf64_Shdr *)(file->data + file->ehdr64->e_shoff);
        if (file->ehdr64->e_shoff == 0 || file->ehdr64->e_shnum == 0)
            error("Invalid section 64 header table");
    }
    else
    {
        file->shdr32 = (Elf32_Shdr *)(file->data + file->ehdr32->e_shoff);

        if (file->ehdr32->e_shoff == 0 || file->ehdr32->e_shnum == 0)
            error("Invalid section 32 header table");
    }
}

// Parse the symbol table of a file and return a pointer to the symbol table
// void parse_symtab32(t_file *file)
// {
//     size_t i;
//     t_syms *syms = &(file->syms);

//     for (i = 0; i < file->ehdr32->e_shnum; i++)
//     {
//         if (file->shdr32[i].sh_type == SHT_SYMTAB)
//         {
//             if (file->shdr32[i].sh_link >= file->ehdr32->e_shnum)
//                 error("Invalid symbol table");

//             syms->symtab = (void *)((char *)file->data + file->shdr32[i].sh_offset);
//             syms->n_syms = file->shdr32[i].sh_size / sizeof(Elf32_Sym);
//             syms->symtab_size = file->shdr32[i].sh_size;
//             syms->strtab = (char *)((char *)file->data + file->shdr32[file->shdr32[i].sh_link].sh_offset);
//             syms->strtab_size = file->shdr32[file->shdr32[i].sh_link].sh_size;

//             break;
//         }
//     }
//     if (syms->symtab == NULL || syms->strtab == NULL)
//         error("No symbol table found");
// }

// void parse_symtab64(t_file *file)
// {
//     size_t i;
//     t_syms *syms = &(file->syms);
//     for (i = 0; i < file->ehdr64->e_shnum; i++)
//     {
//         if (file->shdr64[i].sh_type == SHT_SYMTAB)
//         {
//             syms->n_syms = file->shdr64[i].sh_size / sizeof(Elf64_Sym);
//             syms->symtab = file->data + file->shdr64[i].sh_offset;
//             syms->symtab_size = file->shdr64[i].sh_size;
//             if (file->shdr64[i].sh_link == 0)
//                 error("Invalid symbol table string table index");
//             syms->strtab = file->data + file->shdr64[file->shdr64[i].sh_link].sh_offset;
//             syms->strtab_size = file->shdr64[file->shdr64[i].sh_link].sh_size;
//             break;
//         }
//     }
//     if (syms->n_syms == 0)
//         error("Failed to find symbol table");
// }

void parse_symtab32(t_file *file)
{
    size_t i;
    t_syms *syms = &(file->syms);
    for (i = 0; i < file->ehdr32->e_shnum; i++)
    {
        // char *name = (char *)((void *)file->shdr32 + file->shdr32[file->ehdr32->e_shstrndx].sh_offset) + file->shdr32[i].sh_name;
        if (file->shdr32[i].sh_type == SHT_SYMTAB)
        {
            syms->n_syms = file->shdr32[i].sh_size / sizeof(Elf32_Sym);

            syms->symb = (t_symbol *)malloc(syms->n_syms * sizeof(t_symbol));
            if (syms->symb == NULL)
                error("Failed to allocate memory for symbol table");
            ft_memset(syms->symb, 0, syms->n_syms * sizeof(t_symbol));

            char *strtab = (char *)(file->data + file->shdr32[file->shdr32[i].sh_link].sh_offset);
            syms->strtab = strtab;
            syms->strtab_size = file->shdr32[file->shdr32[i].sh_link].sh_size;
            syms->symtab = file->data + file->shdr32[i].sh_offset;
            syms->symtab_size = file->shdr32[i].sh_size;
            Elf32_Sym *symtab = (Elf32_Sym *)syms->symtab;

            for (size_t j = 0; j < syms->n_syms; j++)
            {
                syms->symb[j].name = strtab + symtab[j].st_name;
                syms->symb[j].type = ELF32_ST_TYPE(symtab[j].st_info);
                
                syms->symb[j].size = symtab[j].st_size;
                syms->symb[j].value = symtab[j].st_value;
                syms->symb[j].section = symtab[j].st_shndx;
            }

            break;
        }
    }
}

void parse_symtab64(t_file *file)
{
    size_t i;
    t_syms *syms = &(file->syms);
    for (i = 0; i < file->ehdr64->e_shnum; i++)
    {
        // Locate the symbol table section
        char *name = (char *)((void *)file->shdr64 + file->shdr64[file->ehdr64->e_shstrndx].sh_offset) + file->shdr64[i].sh_name;
        if (strcmp(name, ".symtab") == 0)
        {
            syms->n_syms = file->shdr64[i].sh_size / sizeof(Elf64_Sym);

            syms->symb = (t_symbol *)malloc(syms->n_syms * sizeof(t_symbol));
            if (syms->symb == NULL)
                error("Failed to allocate memory for symbol table");
            memset(syms->symb, 0, syms->n_syms * sizeof(t_symbol));

            char *strtab = (char *)(file->data + file->shdr64[file->shdr64[i].sh_link].sh_offset);
            syms->strtab = strtab;
            syms->strtab_size = file->shdr64[file->shdr64[i].sh_link].sh_size;
            Elf64_Sym *symtab = NULL;
            syms->symtab = file->data + file->shdr64[i].sh_offset;
            syms->symtab_size = file->shdr64[i].sh_size;
            symtab = (Elf64_Sym *)syms->symtab;
            for (size_t j = 0; j < syms->n_syms; j++)
            {
                syms->symb[j].name = strtab + symtab[j].st_name;
                syms->symb[j].type = ELF64_ST_TYPE(symtab[j].st_info);
               
                syms->symb[j].size = symtab[j].st_size;
                syms->symb[j].value = symtab[j].st_value;
                syms->symb[j].section = symtab[j].st_shndx;
            }
        }
    }
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
        if (files[i].is64)
            parse_symtab64(&files[i]);
        else
            parse_symtab32(&files[i]);
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

void print_symbols(t_file *file)
{
    if (file->ehdr32->e_ident[EI_CLASS] == ELFCLASS32)
    {
        Elf32_Sym *syms32 = (Elf32_Sym *)file->syms.symtab;
        size_t i;

        printf("Symbol table:\n");
        for (i = 0; i < file->syms.n_syms; i++)
        {
            char *name = file->syms.strtab + syms32[i].st_name;
            printf("  %s\n", name);
        }
    }
    else if (file->ehdr32->e_ident[EI_CLASS] == ELFCLASS64)
    {
        Elf64_Sym *syms64 = (Elf64_Sym *)file->syms.symtab;
        size_t i;

        printf("Symbol table:\n");
        for (i = 0; i < file->syms.n_syms; i++)
        {
            char *name = file->syms.strtab + syms64[i].st_name;
            printf("  %s\n", name);
        }
    }
}

// void print_symbolsNew(t_file *file) {
//     printf("Symbol table '%s':\n", file->path);
//     // for (size_t i = 0; i < file->syms.n_syms; i++) {
//     //     printf("%016lx %c %s\n", file->syms.symb[i].value, file->syms.symb[i].type, file->syms.symb[i].name);
//     // }
//     for (size_t i = 0; i < file->syms.n_syms; i++) {
//         printf("file->syms.symb[i].value = %016lx\n", file->syms.symb[i].value);
//         printf("file->syms.symb[i].type = %c\n", file->syms.symb[i].type);
//         printf("file->syms.symb[i].name = %s\n", file->syms.symb[i].name);

//         printf("====================================\n");
//     }
// }

int compare_symbols_by_value(const void *a, const void *b) {
    const t_symbol *sym_a = (const t_symbol *)a;
    const t_symbol *sym_b = (const t_symbol *)b;
    if (sym_a->value < sym_b->value) {
        return -1;
    } else if (sym_a->value > sym_b->value) {
        return 1;
    } else {
        return 0;
    }
}

int compare_symbols_by_name(const void *a, const void *b) {
    t_symbol *sym_a = (t_symbol *)a;
    t_symbol *sym_b = (t_symbol *)b;
    return strcmp(sym_a->name, sym_b->name);
}

void sort_symbols_by_name(t_symbol *symbols, size_t n_symbols) {
    for (size_t i = 1; i < n_symbols; i++) {
        t_symbol temp = symbols[i];
        size_t j = i;
        while (j > 0 && strcmp(symbols[j-1].name, temp.name) > 0) {
            symbols[j] = symbols[j-1];
            j--;
        }
        symbols[j] = temp;
    }
}

void print_symbolsNew(t_file *file) {
    printf("Symbol table '%s':\n", file->path);
    sort_symbols_by_name(file->syms.symb, file->syms.n_syms);
    for (size_t i = 0; i < file->syms.n_syms; i++) {
        printf("%016lx %c %s\n", file->syms.symb[i].value, file->syms.symb[i].type, file->syms.symb[i].name);
    }
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

    if (files[0].is64)
    {
        print_header64(&files[0]);
        print_section_headers(&files[0]);
        // print_symbols(&files[0]);
    }
    else
    {
        // print_header32(&files[0]);
        // print_section_headers(&files[0]);
        // print_symbols(&files[0]);
        // display_symtab(&files[0]);
        print_symbolsNew(files);
    }
    // Close the input files
    close_files(files, argc - 1);

    return 0;
}