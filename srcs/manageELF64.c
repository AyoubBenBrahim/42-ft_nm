
#include "ft_nm.h"

void print_symboles64(t_file *file)
{
    // Print the sorted symbols
    printf("\nSymbol table '%s':\n", file->path);

    // qsort(file->syms.symb, file->syms.n_syms_count, sizeof(t_symb), compare_symbols_by_name);

    if (file->option == NO_SORT)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 0);
    else if (file->option == SORT_ORDER)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
    else if (file->option == REVERSE_SORT)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, -1);
    else if (file->option == UNDEFINED_SYMBOLS_ONLY)
    {
        printf("UNDEFINED_SYMBOLS_ONLY not yet implemented\n");
        exit(1);
    }

    // ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
    for (size_t i = 0; i < file->syms.n_syms_count; i++)
    {
        u_int16_t shndx = file->syms.symb[i].section_tab_index;

        char type = file->syms.symb[i].type_char;
        char *name = file->syms.symb[i].name;
        uint64_t value = file->syms.symb[i].value;
        if (shndx == SHN_ABS || name == NULL || strcmp(name, "") == 0)
            continue;
        if (shndx == SHN_UNDEF)
            printf("%16c %c %s\n", ' ', type, name);
        else
            printf("%016lx %c %s\n", value, type, name);
    }
}


void parse_symtab64(t_file *file)
{
    size_t i;
    t_syms *syms = &file->syms;
    ft_memset(syms, 0, sizeof(t_syms));

    for (i = 0; i < file->ehdr64->e_shnum; i++)
    {
        if (file->shdr64[i].sh_type == SHT_SYMTAB)
        {
            syms->n_syms = file->shdr64[i].sh_size / sizeof(Elf64_Sym);
            syms->symb = malloc(syms->n_syms * sizeof(t_symb));

            if (syms->symb == NULL)
            {
                ft_printf("Failed to allocate memory for symbol table\n");
                exit(1);
            }
            ft_memset(syms->symb, 0, syms->n_syms * sizeof(t_symb));

            char *strtab = (char *)(file->data + file->shdr64[file->shdr64[i].sh_link].sh_offset);
            syms->strtab = strtab;
            syms->strtab_size = file->shdr64[file->shdr64[i].sh_link].sh_size;
            syms->symtab = file->data + file->shdr64[i].sh_offset;
            syms->symtab_size = file->shdr64[i].sh_size;
            Elf64_Sym *symtab = (Elf64_Sym *)syms->symtab;

            for (size_t j = 0; j < syms->n_syms; j++)
            {
                syms->symb[j].name = strtab + symtab[j].st_name;
                syms->symb[j].type = ELF64_ST_TYPE(symtab[j].st_info);
                syms->symb[j].binding = ELF64_ST_BIND(symtab[j].st_info);
                syms->symb[j].visibility = ELF64_ST_VISIBILITY(symtab[j].st_other);

                syms->symb[j].size = symtab[j].st_size;
                syms->symb[j].value = symtab[j].st_value;
                syms->symb[j].section_tab_index = symtab[j].st_shndx;

                syms->symb[j].type_char = determin_symb(file, &syms->symb[j]);
            
                t_symb *symb = &syms->symb[j];

                syms->symb[syms->n_syms_count++] = *symb;
                // store_symb(&file, syms->symb[j].name, syms->symb[j].value, c);
            }
            break;
        }
    }
}

