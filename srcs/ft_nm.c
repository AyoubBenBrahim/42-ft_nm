/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayb***** <ayb*****@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 15:07:36 by ayb*****          #+#    #+#             */
/*   Updated: 2020/01/25 20:17:36 by ayb*****         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_nm.h"
#include <stdio.h>


#define MAX_FILES 10

typedef struct {
    const char *path;
    int fd;
    struct stat stat;
    void *data;
    size_t data_size;
    Elf64_Ehdr *ehdr;  // Pointer to the ELF header
    Elf64_Shdr *shdr;  // Pointer to the section header table
    Elf64_Sym *syms;   // Pointer to the symbol table
    size_t n_syms;     // Number of symbols in the symbol table
} t_file;

// Print an error message and exit the program
void error(char *msg)
{
    fprintf(stderr, "Error: %s (%s)\n", msg, strerror(errno));
    exit(1);
}

int open_file(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        ft_printf("open: '%s': can't open file\n", path);
        error("Failed to open");
    }
    return fd;
}

// Map a file into memory and return a pointer to the mapped data
void *map_file(int fd, size_t size)
{
    void *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        error("mmap: Failed to map file");
    }
    return data;
}

// Unmap a mapped file and close its file descriptor
void unmap_file(void *data, size_t size, int fd)
{
    if (munmap(data, size) == -1) {
        error("Failed to unmap file");
    }
    if (close(fd) == -1) {
        error("Failed to close file");
    }
}

// Parse the ELF header of a file and return a pointer to the ELF header
Elf64_Ehdr *parse_ehdr(void *data)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)data;
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3) {
        error("Invalid ELF header");
    }
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32 &&
        ehdr->e_ident[EI_CLASS] != ELFCLASS64) 
		error("invalid ELF class\n");
    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB &&
        ehdr->e_ident[EI_DATA] != ELFDATA2MSB) {
        error("invalid ELF data encoding\n");
    }
    if (ehdr->e_type != ET_REL && ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN) {
        error("invalid ELF type\n");
    }
    return ehdr;
}

// Parse the section header table of a file and return a pointer to the section header table
Elf64_Shdr *parse_shdr(void *data, Elf64_Ehdr *ehdr)
{
    Elf64_Shdr *shdr = (Elf64_Shdr *)(data + ehdr->e_shoff);
    return shdr;
}

// Parse the symbol table of a file and return a pointer to the symbol table and the number of symbols
void parse_symtab(void *data, Elf64_Ehdr *ehdr, Elf64_Shdr *shdr, Elf64_Sym **syms, size_t *n_syms)
{
    for (int i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type == SHT_SYMTAB) {
            *syms = (Elf64_Sym *)(data + shdr[i].sh_offset);
            *n_syms = shdr[i].sh_size / sizeof(Elf64_Sym);
            return;
        }
    }
    error("Symbol table not found");
}

// Compare function for qsort to sort symbols by address
int cmp_sym_by_addr(const void *a, const void *b)
{
    Elf64_Sym *sym_a = (Elf64_Sym *)a;
    Elf64_Sym *sym_b = (Elf64_Sym *)b;
    if (sym_a->st_value < sym_b->st_value) {
        return -1;
    } else if (sym_a->st_value > sym_b->st_value) {
        return 1;
    } else {
        return 0;
    }
}

// Compare function for qsort to sort symbols by name
int cmp_sym_by_name(const void *a, const void *b)
{
    Elf64_Sym *sym_a = (Elf64_Sym *)a;
    Elf64_Sym *sym_b = (Elf64_Sym *)b;
    return strcmp((char *)sym_a + sym_a->st_name, (char *)sym_b + sym_b->st_name);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        ft_printf("Usage: ft_nm <file> [<file> ...]\n");
        return 1;
        // error("Usage: ft_nm <file> [<file> ...]");
    }
    
    // Step 1: Parsing command-line arguments
    int n_files = argc - 1;
    if (n_files > MAX_FILES) {
        error("Too many files");
    }
    t_file files[MAX_FILES];
    ft_memset(files, 0, sizeof(files));
    for (int i = 0; i < n_files; i++) {
        files[i].path = argv[i+1];
    }
    
    // Step 2: Creating an array of t_file structures to hold information about each file
    for (int i = 0; i < n_files; i++) {
        files[i].fd = open_file(files[i].path);
    }
    
    // Step 3: Opening each file
    for (int i = 0; i < n_files; i++) {
        if (fstat(files[i].fd, &files[i].stat) == -1) {
            ft_printf("fstat: '%s': can't stat file\n", files[i].path);
            exit(1);
        }
        files[i].data_size = files[i].stat.st_size;
        files[i].data = map_file(files[i].fd, files[i].stat.st_size);
    }
    
    // for (int i = 0; i < n_files; i++) {
    //     ft_printf("%x\n", files[i].data);
    //     ft_printf("%d\n", files[i].data_size);
    // }
    // exit(0);

    // Step 4: Mapping each file into memory
    for (int i = 0; i < n_files; i++) {
        files[i].ehdr = parse_ehdr(files[i].data);
        files[i].shdr = parse_shdr(files[i].data, files[i].ehdr);
        parse_symtab(files[i].data, files[i].ehdr, files[i].shdr, &files[i].syms, &files[i].n_syms);
    }
    
    // Step 5: Processing each file
    for (int i = 0; i < n_files; i++) {
        // Do something with files[i].syms
    }
    
    // Step 6: Sorting the symbols
    for (int i = 0; i < n_files; i++) {
        qsort(files[i].syms, files[i].n_syms, sizeof(Elf64_Sym), cmp_sym_by_name);
    }
    
    // Step 7: Printing the symbols
    for (int i = 0; i < n_files; i++) {
        printf("%s:\n", files[i].path);
        for (size_t j = 0; j < files[i].n_syms; j++) {
            printf("%016llx %c %s\n", files[i].syms[j].st_value, '?', (char *)files[i].data + files[i].syms[j].st_name);
        }
    }
    
    // Clean up
    for (int i = 0; i < n_files; i++) {
        unmap_file(files[i].data, files[i].data_size, files[i].fd);
    }
    
    return 0;
}
