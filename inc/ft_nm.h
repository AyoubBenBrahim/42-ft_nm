/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayb***** <ayb*****@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 15:08:03 by ayb*****          #+#    #+#             */
/*   Updated: 2020/01/25 20:17:18 by ayb*****         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
// # include <linux/elf.h>
// #include "../inc/elf.h"

#include <stdbool.h>

#include <errno.h>

#include <stdio.h>

# define TO_LOWER(x)		(x | ' ')


typedef struct
{
    char *name;                 // Symbol name
    uint64_t size;              // Symbol size
    uint64_t value;             // Symbol value
    uint16_t section_tab_index; // Section index

    uint8_t type;
    uint8_t binding;
    uint8_t visibility;

    char type_char;
    
    // uint32_t flags;
} t_symb;

typedef struct
{
    size_t n_syms;      // Number of symbols in the symbol table
    size_t n_syms_count;  // Number of symbols actually stored
    
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

    t_syms syms; //Pointer to the symbol table and string table information

    // char *final_sym_name;
    // uint64_t final_sym_addr;
    // char final_sym_type;
} t_file;

/*
**** Debugging functions
*/

// void print_header32(t_file *file);
// void print_header64(t_file *file);

// void print_section_headers(t_file *file);
// void print_symbols(t_file *file);

// void display_symtab(t_file *file);

// char *get_symbol_type(unsigned char info);
// char *get_symbol_bind(unsigned char info);
// char *get_symbol_visibility(unsigned char other);

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
void unmap_file(void *data, size_t data_size, int fd);
void *map_file(int fd, size_t size);
int open_file(const char *path);

void error(const char *msg);




// char determin_symb(t_file *file, t_syms *syms, t_symbol *symb);
char determin_symb(t_file *file, t_symb *symb);
// char get_symb_type_and_visibility(t_file *file);
char get_symb_type_and_visibility(t_symb *symb);
char get_symbol_section_char(uint16_t section, t_file *file);
char get_symbol_binding_char(uint8_t binding);
char get_symbol_type_char(uint32_t type);
void sort_symbols_by_name(t_symb *symbols, size_t n_symbols);
int compare_symbols_by_name(const void *a, const void *b);
// void store_symb(t_file **file, char *name, uint64_t st_value, char type);


void print_symboles32(t_file *file);

#endif
