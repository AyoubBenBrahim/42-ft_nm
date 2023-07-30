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
            error("he file has no section header table");
    }
    else
    {
        file->shdr32 = (Elf32_Shdr *)(file->data + file->ehdr32->e_shoff);

        if (file->ehdr32->e_shoff == 0 || file->ehdr32->e_shnum == 0)
            error("The file has no section header table");
    }
}

void parse_symtab32(t_file *file)
{
    size_t i;
    t_syms *syms = &file->syms;
    ft_memset(syms, 0, sizeof(t_syms));
    // syms->n_syms_count = 0;
    // syms->n_syms = 0;
    // syms->symb = NULL;

    for (i = 0; i < file->ehdr32->e_shnum; i++)
    {
        // char *name = (char *)((void *)file->shdr32 + file->shdr32[file->ehdr32->e_shstrndx].sh_offset) + file->shdr32[i].sh_name;
        if (file->shdr32[i].sh_type == SHT_SYMTAB)
        {
            syms->n_syms = file->shdr32[i].sh_size / sizeof(Elf32_Sym);

            // printf("syms->n_syms = %ld\n", syms->n_syms); // SEGV when deleting this line or when using ft_printf

            syms->symb = malloc(syms->n_syms * sizeof(t_symb));
            // syms->symb = (t_symb *)malloc(syms->n_syms * sizeof(t_symb));

            if (syms->symb == NULL)
                error("Failed to allocate memory for symbol table");
            ft_memset(syms->symb, 0, syms->n_syms * sizeof(t_symb));

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
                syms->symb[j].binding = ELF32_ST_BIND(symtab[j].st_info);
                syms->symb[j].visibility = ELF32_ST_VISIBILITY(symtab[j].st_other);

                syms->symb[j].size = symtab[j].st_size;
                syms->symb[j].value = symtab[j].st_value;
                syms->symb[j].section_tab_index = symtab[j].st_shndx;

                // char c = determin_symb(file, &syms->&syms->ymb[j]);
                syms->symb[j].type_char = determin_symb(file, &syms->symb[j]);
                t_symb *symb = &syms->symb[j];

                syms->symb[syms->n_syms_count++] = *symb;
                // store_symb(&file, syms->symb[j].name, syms->symb[j].value, c);

                // if (file->final_sym_addr != 0)
                //     printf("%08lx %c %s\n", file->final_sym_addr, file->final_sym_type, file->final_sym_name);
                // else
                //     printf("%8c %c %s\n", ' ', file->final_sym_type, file->final_sym_name);

                // if (syms->symb[j].value != 0)
                //     printf("%08lx %c %s\n", syms->symb[j].value, c, syms->symb[j].name);
                // else
                //     printf("%8c %c %s\n", ' ', c, syms->symb[j].name);
            }

            // sort_symboles32(syms->symb, syms->n_syms);
            // print_symboles32(file);

            break;
        }
        // return syms;
        // print_symboles32(file);
    }
}

void print_symboles32(t_file *file)
{
    // Print the sorted symbols
    printf("\nSymbol table '%s':\n", file->path);

    for (size_t i = 0; i < file->syms.n_syms_count; i++)
    {
        u_int16_t shndx = file->syms.symb[i].section_tab_index;

        char type = file->syms.symb[i].type_char;
        char *name = file->syms.symb[i].name;
        uint32_t value = file->syms.symb[i].value;
        if (shndx == SHN_ABS || name == NULL || strcmp(name, "") == 0)
            continue;
        if (shndx == SHN_UNDEF)
            printf("%8c %c %s\n", ' ', type, name);
        else
            printf("%08x %c %s\n", value, type, name);
    }
}

void parse_symtab64(t_file *file)
{

    (void)file;
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
            error("Failed to get file size");

        get_data_and_size(&files[i]);
        parse_ehdr(&files[i]);
        parse_shdr(&files[i]);
        if (files[i].is64)
            parse_symtab64(&files[i]);
        else
        {
            parse_symtab32(&files[i]);
            //  sort
            print_symboles32(&files[i]);
        }
    }
}

// Close multiple files and unmap them from memory
void close_files(t_file files[], int n_files)
{
    int i;
    for (i = 0; i < n_files; i++)
        unmap_file(files[i].data, files[i].data_size, files[i].fd);
}

// void print_symbols(t_file *file)
// {
//     if (file->ehdr32->e_ident[EI_CLASS] == ELFCLASS32)
//     {
//         Elf32_Sym *syms32 = (Elf32_Sym *)file->syms.symtab;
//         size_t i;

//         printf("Symbol table:\n");
//         for (i = 0; i < file->syms.n_syms; i++)
//         {
//             char *name = file->syms.strtab + syms32[i].st_name;
//             printf("  %s\n", name);
//         }
//     }
//     else if (file->ehdr32->e_ident[EI_CLASS] == ELFCLASS64)
//     {
//         Elf64_Sym *syms64 = (Elf64_Sym *)file->syms.symtab;
//         size_t i;

//         printf("Symbol table:\n");
//         for (i = 0; i < file->syms.n_syms; i++)
//         {
//             char *name = file->syms.strtab + syms64[i].st_name;
//             printf("  %s\n", name);
//         }
//     }
// }

int compare_symbols_by_name(const void *a, const void *b)
{
    t_symb *sym_a = (t_symb *)a;
    t_symb *sym_b = (t_symb *)b;
    return strcmp(sym_a->name, sym_b->name);
}

void sort_symbols_by_name(t_symb *symbols, size_t n_symbols)
{
    for (size_t i = 1; i < n_symbols; i++)
    {
        t_symb temp = symbols[i];
        size_t j = i;
        while (j > 0 && strcmp(symbols[j - 1].name, temp.name) > 0)
        {
            symbols[j] = symbols[j - 1];
            j--;
        }
        symbols[j] = temp;
    }
}

/*
*** Function to determine the character corresponding to symbol section
*** The & operator is used to check if a certain flag is set in the section flags,
*** while the | operator is used to check if a combination of flags is set.
*/
char get_symbol_section_char(uint16_t section, t_file *file)
{
    uint32_t sh_type = file->shdr32[section].sh_type;
    uint32_t sh_flags = file->shdr32[section].sh_flags;
    char c = '?';

    switch (sh_type)
    {
    case SHT_NOBITS:
        if (sh_flags & (SHF_ALLOC | SHF_WRITE))
        {
            if (sh_flags & SHF_IA_64_SHORT)
                c = 'S';
            else
                c = 'B';
        }
        break;
    case SHT_IA_64_UNWIND:
        c = 'p';
        break;
    case SHT_PROGBITS:
        if (sh_flags & SHF_ALLOC)
        {
            if (sh_flags & SHF_WRITE)
            {
                if (sh_flags & SHF_IA_64_SHORT)
                    c = 'G';
                else
                    c = 'D';
            }
            else if (sh_flags & SHF_EXECINSTR)
                c = 'T';
            else
            {
                if (sh_type == SHT_NOBITS)
                    c = 'n';
                else
                    c = 'R';
            }
        }
        break;
    case SHT_NULL:
        c = 'N';
        break;
    case SHT_RELA:
    case SHT_REL:
    case SHT_HASH:
    case SHT_GNU_versym:
    case SHT_GNU_verdef:
    case SHT_STRTAB:
    case SHT_DYNSYM:
    case SHT_NOTE:
        if (sh_flags & SHF_ALLOC)
            c = 'R';
        break;
    case SHT_DYNAMIC:
    case SHT_FINI_ARRAY:
    case SHT_INIT_ARRAY:
        c = 'D';
        break;
    case SHT_MIPS_SYMBOL_LIB:
        c = 'T';
        break;
    case SHT_GROUP:
        c = 'N';
        break;
    case SHT_GNU_verneed: //
        c = 'R';          //
        break;            //
    default:
        c = '?';
        break;
    }
    return c;
}

char get_symb_type_and_visibility(t_symb *symb)
{
    uint16_t shndx = symb->section_tab_index; // t_shndx
    // uint32_t type = ELF32_ST_TYPE(symb->type);    // THIS IS THE STUPIDEST THING I'VE EVER DONE
    uint32_t type = symb->type;
    uint8_t binding = symb->binding;
    uint8_t visibility = symb->visibility;

    // if (shndx == SHN_ABS)
    //     return 'A';
    // if (shndx == SHN_COMMON)
    //     return 'C';
    // if (type == STT_GNU_IFUNC)
    //     return 'i';
    // if (binding == STB_WEAK)
    // {
    //     if (type == STT_OBJECT)
    //         return shndx == SHN_UNDEF ? 'v' : 'V';
    //     else
    //         return shndx == SHN_UNDEF ? 'w' : 'W';
    // }
    // if (binding == STB_GNU_UNIQUE)
    //     return 'u';
    // if (shndx == SHN_UNDEF)
    //     return 'U';
    // return '?';

    char c = '?';
    if (binding == STB_GNU_UNIQUE)
        c = 'u';
    else if (binding == STB_WEAK && type == STT_OBJECT)
    {
        c = 'V';
        if (shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (binding == STB_WEAK && visibility == STV_DEFAULT) //(binding == STB_WEAK)
    {
        c = 'W';
        if (shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (shndx == SHN_UNDEF)
        c = 'U';
    else if (shndx == SHN_ABS)
        c = 'A';
    else if (shndx == SHN_COMMON)
        c = 'C';
    else if (type == STT_GNU_IFUNC)
        c = 'i';

    return c;
}

// void store_symb(t_file **file, char *name, uint64_t st_value, char type)
// {
//     (*file)->final_sym_name = name;
//     (*file)->final_sym_addr = st_value;
//     (*file)->final_sym_type = type;
// }

char determin_symb(t_file *file, t_symb *symb)
{
    char c = get_symb_type_and_visibility(symb);
    if (c == '?')
        c = get_symbol_section_char(symb->section_tab_index, file);

    if (symb->binding == STB_LOCAL)
        c = TO_LOWER(c);
    return c;
}

void symboles_printer(t_file *file)
{
    printf("\n%s:\n", file->path);
    // qsort(file->syms.symb, file->syms.n_syms, sizeof(t_symb), compare_symbols_by_name);
    // for (size_t i = 0; i < file->syms->n_syms; i++)
    // {
    // printf("T22 = %d\n", file->syms.symb[i].type);
    // char type = get_symbol_type_char(file->syms.symb[i].type);
    // char binding = get_symbol_binding_char(file->syms.symb[i].binding);
    // // printf("binding = %c\n", binding);

    // char section = get_symbol_section_char(file->syms.symb[i].section_tab_index, file);
    // // printf("section = %c\n", section);
    // if (section != 'x')
    //     type = section;
    // char c = determin_symb(file);

    // determin_symb(file, &file->syms, &file->syms.symb[i]);

    // elf_constructor(&file, file->syms.symb[i].name, file->syms.symb[i].value, c);

    // if (file->syms.symb[i].section_tab_index != SHN_UNDEF)
    // printf("%08lx %c %s\n", (unsigned long)file->syms.symb[i].value, c, file->syms.symb[i].name);

    // printf("%016lx %c %s\n", file->final_sym_addr, file->final_sym_type, file->final_sym_name);
    // }
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
    // print_type_binding(&files[0]);
    // exit(1);
    if (files[0].is64)
    {
        // print_header64(&files[0]);
        // print_section_headers(&files[0]);
        // print_symbols(&files[0]);
    }
    else
    {
        // printf("%s\n", files[0]->syms->symb[0].name);
        // print_header32(&files[0]);
        // print_section_headers(&files[0]);
        // print_symbols(&files[0]);

        // display_symtab(&files[0]);
        // print_symbolsNEW2(files);
        // print_type_binding(&files[0]);
        // symboles_printer(&files[0]);
    }
    // Close the input files
    close_files(files, argc - 1);

    return 0;
}