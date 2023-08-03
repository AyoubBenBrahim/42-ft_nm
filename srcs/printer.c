#include "ft_nm.h"

void print_symboles32(t_file *file)
{
    // printf("\nSymbol table '%s':\n", file->path);

    // qsort(file->syms.symb, file->syms.n_syms_count, sizeof(t_symb), compare_symbols_by_name);

    if (file->option == NO_SORT)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 0);
    else if (file->option == SORT_ORDER)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
    else if (file->option == REVERSE_SORT)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, -1);
    else if (file->option == UNDEFINED_SYMBOLS_ONLY)
    {
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char *name = file->syms.symb[i].name;

            if (shndx == SHN_UNDEF && ft_strcmp(name, "")) // && (type == 'U' || type == 'w'))
                ft_printf("%8c %c %s\n", ' ', type, name);
        }
        return;
    }
    else if (file->option == DISPLAY_ALL)
    {
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char *name = file->syms.symb[i].name;
            uint32_t value = file->syms.symb[i].value;
            if (shndx == SHN_UNDEF && ft_strcmp(name, "") == 0)
                continue;
            if (shndx == SHN_UNDEF)
                ft_printf("%8c %c %s\n", ' ', type, name);
            else
                ft_printf("%08x %c %s\n", value, type, name);
        }

        return;
    }

    // ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
    for (size_t i = 0; i < file->syms.n_syms_count; i++)
    {
        u_int16_t shndx = file->syms.symb[i].section_tab_index;

        char type = file->syms.symb[i].type_char;
        char *name = file->syms.symb[i].name;
        uint32_t value = file->syms.symb[i].value;
        if (shndx == SHN_ABS || name == NULL || ft_strcmp(name, "") == 0)
            continue;
        if (shndx == SHN_UNDEF)
            ft_printf("%8c %c %s\n", ' ', type, name);
        else
            ft_printf("%08x %c %s\n", value, type, name);
    }
}

void print_symboles64(t_file *file)
{
    if (file->option == NO_SORT)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 0);
    else if (file->option == SORT_ORDER)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
    else if (file->option == REVERSE_SORT)
        ft_qsort(file->syms.symb, file->syms.n_syms_count, -1);
    else if (file->option == UNDEFINED_SYMBOLS_ONLY)
    {
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char *name = file->syms.symb[i].name;
            // uint32_t value = file->syms.symb[i].value;
            // if (shndx == SHN_UNDEF || name == NULL || strcmp(name, "") == 0)
            //     continue;
            if (shndx == SHN_UNDEF && ft_strcmp(name, "")) // && (type == 'U' || type == 'w'))
                ft_printf("%16c %c %s\n", ' ', type, name);
        }

        return;
    }
    else if (file->option == DISPLAY_ALL)
    {
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
        // ft_printf("UNDEFINED_SYMBOLS_ONLY not yet implemented\n");
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char *name = file->syms.symb[i].name;
            uint64_t value = file->syms.symb[i].value;

            if (shndx == SHN_UNDEF && ft_strcmp(name, "") == 0)
                continue;
            if (shndx == SHN_UNDEF)
                ft_printf("%16c %c %s\n", ' ', type, name);
            else
                ft_printf("%016lx %c %s\n", value, type, name);
        }

        return;
    }

    // ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
    for (size_t i = 0; i < file->syms.n_syms_count; i++)
    {
        u_int16_t shndx = file->syms.symb[i].section_tab_index;

        char type = file->syms.symb[i].type_char;
        char *name = file->syms.symb[i].name;
        uint64_t value = file->syms.symb[i].value;
        if (shndx == SHN_ABS || name == NULL || ft_strcmp(name, "") == 0)
            continue;
        if (shndx == SHN_UNDEF)
            ft_printf("%16c %c %s\n", ' ', type, name);
        else
            ft_printf("%016lx %c %s\n", value, type, name);
    }
}
