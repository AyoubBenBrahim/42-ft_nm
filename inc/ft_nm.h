
#ifndef FT_NM_H
#define FT_NM_H

#include "../libft/libft.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdbool.h>
#include <errno.h>
// #include <stdio.h>

#define TO_LOWER(x) (x | ' ')

#define MAX_FILES 12

typedef enum
{
    NO_SORT,                // p
    SORT_ORDER,             // Default
    REVERSE_SORT,           // r
    DISPLAY_ALL,            // a
    UNDEFINED_SYMBOLS_ONLY, // u
    EXTERNAL_ONLY           // g
} e_option;

typedef struct
{
    char *name;                 // Symbol name
    uint64_t size;              // Symbol size
    uint64_t value;             // Symbol address
    uint16_t section_tab_index; // Section index
    const char *section_name;

    uint8_t type;
    uint8_t binding;
    uint8_t visibility;

    char type_char;

    uint32_t sh_type; // Section type (SYMTAB, STRTAB, etc.)

} t_symb;

typedef struct
{
    size_t n_syms;       // Number of symbols in the symbol table
    size_t n_syms_count; // Number of symbols actually stored

    void *symtab;       // Pointer to the symbol table
    size_t symtab_size; // Size of the symbol table in bytes
    char *strtab;       // Pointer to the string table
    size_t strtab_size; // Size of the string table in bytes

    t_symb *symb; // Pointer to the symbol table

} t_syms;

typedef struct
{
    const char *path;
    int fd;
    struct stat stat;
    void *data;
    size_t data_size;

    bool is64;

    Elf64_Shdr *shdr64;
    Elf64_Ehdr *ehdr64;

    Elf32_Shdr *shdr32;
    Elf32_Ehdr *ehdr32;

    t_syms syms; // Pointer to the symbol table and string table information

    e_option option;
} t_file;

/*
**** Debugging functions
*/

void print_header32(t_file *file);

/*
**** Parsing functions
*/

void get_data_and_size(t_file *file);
void manageELF(t_file files[], int n_files);
void close_files(t_file files[], int n_files);
void parse_ehdr(t_file *file);
void parse_shdr(t_file *file);
void parse_symtab32(t_file *file);
void parse_symtab64(t_file *file);
void unmap_file(const char *path, void *data, size_t size, int fd);
void *map_file(const char *path, int fd, size_t size);
int open_file(const char *path);

/*
**** Symbol table handling functions
*/

char determine_symb(t_file *file, t_symb *symb);
char get_symb_type_and_visibility(t_symb *symb);
char get_symbol_section_char(uint16_t section, t_file *file);
char get_symbol_binding_char(uint8_t binding);
char get_symbol_type_char(uint32_t type);
char get_symb_based_on_section(t_file *file, uint16_t section, const char *name);
int get_section_name(uint32_t sh_flag, const char *name);

/*
**** Printing functions
*/

void ft_qsort(t_symb *array, size_t size, int order);
void mergeSort(t_symb *array, int size, int order);
void undifined_symbols_only(t_file *file);
void display_all(t_file *file);
void external_only(t_file *file);
void no_flag(t_file *file);
void print_symboles(t_file *file, int n_files);
void output(t_file *file, uint64_t value, char type, char *name, u_int16_t shndx);

#endif
