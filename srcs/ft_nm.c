#include "ft_nm.h"

int open_file(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        ft_printf("[ERROR] can't open file : '%s'\n", path);
        exit(1);
    }
    return fd;
}

// Map a file into memory and return a pointer to the mapped data
void *map_file(const char *path, int fd, size_t size)
{
    void *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED)
    {
        ft_printf("mmap: Failed to map file '%s'\n", path);
        exit(1);
    }
    return data;
}

void unmap_file(const char *path, void *data, size_t size, int fd)
{
    if (munmap(data, size) == -1)
    {
        ft_printf("unmap: Failed to unmap file '%s'\n", path);
        exit(1);
    }

    if (close(fd) == -1)
    {
        ft_printf("close fd: Failed to close file '%s'\n", path);
        exit(1);
    }
}

void parse_ehdr(t_file *file)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)(file->data);

    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3)
    {
        ft_printf("Invalid ELF header/magic number\n");
        exit(1);
    }
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32 && ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
        ft_printf("Invalid ELF class/arch\n");
        exit(1);
    }

    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB && ehdr->e_ident[EI_DATA] != ELFDATA2MSB)
    {
        ft_printf("Invalid ELF data encoding(LSB/MSB)\n");
        exit(1);
    }

    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN && ehdr->e_type != ET_CORE && ehdr->e_type != ET_NONE)
    {
        ft_printf("Invalid ELF type\n");
        exit(1);
    }

    if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
    {
        file->is64 = false;
        file->ehdr32 = (Elf32_Ehdr *)(file->data);
        if (file->ehdr32->e_shoff == 0)
        {
            ft_printf("The file has no section header table\n");
            exit(1);
        }
    }
    else
    {
        file->is64 = true;
        file->ehdr64 = (Elf64_Ehdr *)(file->data);
        if (file->ehdr64->e_shoff == 0)
        {
            ft_printf("The file has no section header table\n");
            exit(1);
        }
    }
}

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
        {
            ft_printf("The file has no section header table\n");
            exit(1);
        }
    }
    else
    {
        file->shdr32 = (Elf32_Shdr *)(file->data + file->ehdr32->e_shoff);

        if (file->ehdr32->e_shoff == 0 || file->ehdr32->e_shnum == 0)
        {
            ft_printf("The file has no section header table\n");
            exit(1);
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
    file->data = map_file(file->path, file->fd, file->stat.st_size);
}

void manageELF(t_file files[], int n_files)
{
    int i;
    for (i = 0; i < n_files; i++)
    {
        files[i].fd = open_file(files[i].path);
        if (fstat(files[i].fd, &files[i].stat) == -1)
        {
            ft_printf("fstat: '%s': Failed to get file size\n", files[i].path);
            exit(1);
        }

        get_data_and_size(&files[i]);
        parse_ehdr(&files[i]);
        // print_header32(&files[i]);
        parse_shdr(&files[i]);
        if (files[i].is64)
        {
            parse_symtab64(&files[i]);
            if (files[i].ehdr64->e_shoff > (size_t)files[i].stat.st_size)
            {
                // "section header table goes past the end of the file"
                ft_printf("Invalid ELF section header offset\n");
                exit(1);
            }
            print_symboles64(&files[i], n_files);
            free(files[i].syms.symb);
        }
        else
        {
            parse_symtab32(&files[i]);
            if (files[i].ehdr32->e_shoff > (size_t)files[i].stat.st_size)
            {
                ft_printf("Invalid ELF section header offset\n");
                exit(1);
            }
            print_symboles32(&files[i], n_files);
            free(files[i].syms.symb);
        }
    }
}

void close_files(t_file files[], int n_files)
{
    int i;
    for (i = 0; i < n_files; i++)
        unmap_file(files[i].path, files[i].data, files[i].data_size, files[i].fd);
}

int main(int argc, char *argv[])
{
    t_file files[MAX_FILES];
    int i, n_files;

    ft_memset(files, 0, sizeof(files));
    // [p r u a g]
    //  -p     Don't sort; display in symbol-table order.
    //  -r     Sort in reverse order.
    //  -u     Display only undefined symbols.
    //  -a     Display all symbols.
    e_option option = SORT_ORDER;
    for (i = 1, n_files = 0; i < argc; i++)
    {
        if (ft_strcmp(argv[i], "-p") == 0)
            option = NO_SORT;
        else if (ft_strcmp(argv[i], "-r") == 0)
            option = REVERSE_SORT;
        else if (ft_strcmp(argv[i], "-u") == 0)
            option = UNDEFINED_SYMBOLS_ONLY;
        else if (ft_strcmp(argv[i], "-a") == 0)
            option = DISPLAY_ALL;
        else if (ft_strcmp(argv[i], "-g") == 0)
            option = EXTERNAL_ONLY;
        else
        {
            if (n_files >= MAX_FILES)
            {
                ft_printf("Error: too many input files (maximum is %d)\n", MAX_FILES);
                exit(1);
            }
            files[n_files].path = argv[i];
            files[n_files].option = option;
            n_files++;
        }
    }
    if (n_files == 0)
    {
        files[n_files].path = "a.out";
        files[n_files].option = option;
        n_files++;
    }

    manageELF(files, n_files);
    close_files(files, n_files);

    return 0;
}