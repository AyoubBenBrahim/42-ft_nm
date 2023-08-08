#include "ft_nm.h"

void print_symboles32(t_file *file, int n_files)
{
    if (n_files > 1)
        ft_printf("\n%s:\n", file->path);

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
    else if (file->option == EXTERNAL_ONLY)
    {
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char type_as_str[2] = {type, '\0'};
            char *name = file->syms.symb[i].name;
            uint32_t value = file->syms.symb[i].value;
            if ((shndx == SHN_UNDEF && ft_strcmp(name, "") == 0) || (ft_strpbrk(type_as_str, "RWwDBUTA") == NULL))
                continue;
            if (shndx == SHN_UNDEF)
                ft_printf("%8c %c %s\n", ' ', type, name);
            else
                ft_printf("%08x %c %s\n", value, type, name);
        }
        return;
    }


    for (size_t i = 0; i < file->syms.n_syms_count; i++)
    {
        u_int16_t shndx = file->syms.symb[i].section_tab_index;

        char type = file->syms.symb[i].type_char;
        char *name = file->syms.symb[i].name;
        uint32_t value = file->syms.symb[i].value;
        if (type == 'a' || name == NULL || ft_strcmp(name, "") == 0)
            continue;
        if (shndx == SHN_UNDEF)
            ft_printf("%8c %c %s\n", ' ', type, name);
        else
            ft_printf("%08x %c %s\n", value, type, name);
    }
}

void print_symboles64(t_file *file, int n_files)
{
    if (n_files > 1)
        ft_printf("\n%s:\n", file->path);
     
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
                ft_printf("%16c %c %s\n", ' ', type, name);
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
    else if (file->option == EXTERNAL_ONLY)
    {
        ft_qsort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char type_as_str[2] = {type, '\0'};
            char *name = file->syms.symb[i].name;
            uint64_t value = file->syms.symb[i].value;
            if ((shndx == SHN_UNDEF && ft_strcmp(name, "") == 0) || (ft_strpbrk(type_as_str, "RWwDBUTA") == NULL))
                continue;
             if (shndx == SHN_UNDEF)
                ft_printf("%16c %c %s\n", ' ', type, name);
            else
                ft_printf("%016lx %c %s\n", value, type, name);
        }
        return;
    }

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

void ft_qsort(t_symb *array, size_t size, int order)
{
    if (size <= 1 || array == NULL || order == 0)
    {
        return;
    }
    t_symb pivot = array[size / 2];
    t_symb *left = array;
    t_symb *right = array + size - 1;
    while (left <= right)
    {
        while (order * ft_strcmp(left->name, pivot.name) < 0)
        {
            left++;
        }
        while (order * ft_strcmp(right->name, pivot.name) > 0)
        {
            right--;
        }
        if (left <= right)
        {
            t_symb tmp = *left;
            *left = *right;
            *right = tmp;
            left++;
            right--;
        }
    }
    ft_qsort(array, right - array + 1, order);
    ft_qsort(left, array + size - left, order);
}
