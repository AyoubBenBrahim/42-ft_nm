#include "ft_nm.h"

void undifined_symbols_only(t_file *file)
{
    if (file->option == UNDEFINED_SYMBOLS_ONLY)
    {
        mergeSort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;
            char type = file->syms.symb[i].type_char;
            char *name = file->syms.symb[i].name;

            if (file->is64)
            {
                if (shndx == SHN_UNDEF && ft_strcmp(name, ""))
                    ft_printf("%16c %c %s\n", ' ', type, name);
                continue;
            }

            if (shndx == SHN_UNDEF && ft_strcmp(name, ""))
                ft_printf("%8c %c %s\n", ' ', type, name);
        }
        return;
    }
}

void display_all(t_file *file)
{
    if (file->option == DISPLAY_ALL)
    {
        mergeSort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char *name = file->syms.symb[i].name;
            uint64_t value = file->syms.symb[i].value;
            if (shndx == SHN_UNDEF && ft_strcmp(name, "") == 0)
                continue;

            output(file, value, type, name, shndx);
        }
        return;
    }
}

void external_only(t_file *file)
{
    if (file->option == EXTERNAL_ONLY)
    {
        mergeSort(file->syms.symb, file->syms.n_syms_count, 1);
        for (size_t i = 0; i < file->syms.n_syms_count; i++)
        {
            u_int16_t shndx = file->syms.symb[i].section_tab_index;

            char type = file->syms.symb[i].type_char;
            char type_as_str[2] = {type, '\0'};
            char *name = file->syms.symb[i].name;
            uint64_t value = file->syms.symb[i].value;

            if ((ft_strpbrk(type_as_str, "RWwDBUTAVi") == NULL) || file->syms.symb[i].binding == STB_LOCAL) // only print external symbols aka global
                continue;

            output(file, value, type, name, shndx);
        }
        return;
    }
}

void no_flag(t_file *file)
{
    if (!is_in_range(file->option, 0, 2)) // NO_SORT, SORT_ORDER, REVERSE_SORT
        return;
    for (size_t i = 0; i < file->syms.n_syms_count; i++)
    {
        u_int16_t shndx = file->syms.symb[i].section_tab_index;
        char type = file->syms.symb[i].type_char;
        char *name = file->syms.symb[i].name;
        uint64_t value = file->syms.symb[i].value;

        uint32_t section_type = file->syms.symb[i].sh_type;

        if (ft_strcmp(name, "") == 0 && !value)
            continue;
        // SHN_ABS ignores both A and a
        // if ((shndx == SHN_ABS && !value) || section_type != SHT_SYMTAB)
        //     continue;
        if ((type == 'a' && !value) || section_type != SHT_SYMTAB)
            continue;

        output(file, value, type, name, shndx);
    }
}

void output(t_file *file, uint64_t value, char type, char *name, u_int16_t shndx)
{
    if (file->is64)
    {
        if (shndx == SHN_UNDEF)
            ft_printf("%16c %c %s\n", ' ', type, name);
        else
            ft_printf("%016lx %c %s\n", value, type, name);

        return;
    }

    if (shndx == SHN_UNDEF)
        ft_printf("%8c %c %s\n", ' ', type, name);
    else
        ft_printf("%08x %c %s\n", value, type, name);
}

void print_symboles(t_file *file, int n_files)
{
    if (n_files > 1)
        ft_printf("\n%s:\n", file->path);

    if (file->option == NO_SORT)
        mergeSort(file->syms.symb, file->syms.n_syms_count, 0);
    else if (file->option == SORT_ORDER)
        mergeSort(file->syms.symb, file->syms.n_syms_count, 1);
    else if (file->option == REVERSE_SORT)
        mergeSort(file->syms.symb, file->syms.n_syms_count, -1);

    undifined_symbols_only(file);
    display_all(file);
    external_only(file);
    no_flag(file);
}

/*
**************************************************************
*/

/*
*** A sorting algorithm is said to be stable if two elements with equal keys
*** appear in the same order in sorted output as they appear in the input data set
*/

void merge(t_symb *array, t_symb *left, int leftSize, t_symb *right, int rightSize, int order)
{
    int i = 0, j = 0, k = 0;
    while (i < leftSize && j < rightSize)
    {
        if (order * ft_strcmp(left[i].name, right[j].name) <= 0)
            array[k++] = left[i++];
        else
            array[k++] = right[j++];
    }
    while (i < leftSize)
        array[k++] = left[i++];
    while (j < rightSize)
        array[k++] = right[j++];
}

void mergeSort(t_symb *array, int size, int order)
{
    if (size <= 1)
        return;

    int mid = size / 2;
    int leftSize = mid;
    int rightSize = size - mid;

    t_symb *left = (t_symb *)malloc(leftSize * sizeof(t_symb));
    t_symb *right = (t_symb *)malloc(rightSize * sizeof(t_symb));

    ft_memcpy(left, array, leftSize * sizeof(t_symb));
    ft_memcpy(right, array + leftSize, rightSize * sizeof(t_symb));

    mergeSort(left, leftSize, order);
    mergeSort(right, rightSize, order);

    merge(array, left, leftSize, right, rightSize, order);

    free(left);
    free(right);
}

/*
**************************************************************
*/

// quick sort is not a stable sorting algorithm

// void ft_qsort(t_symb *array, size_t size, int order)
// {
//     if (size <= 1 || array == NULL || order == 0)
//         return;

//     t_symb pivot = array[size / 2];
//     t_symb *left = array;
//     t_symb *right = array + size - 1;
//     while (left <= right)
//     {
//         while (order * ft_strcmp(left->name, pivot.name) < 0)
//             left++;
//         while (order * ft_strcmp(right->name, pivot.name) > 0)
//             right--;
//         if (left <= right)
//         {
//             t_symb tmp = *left;
//             *left = *right;
//             *right = tmp;
//             left++;
//             right--;
//         }
//     }
//     ft_qsort(array, right - array + 1, order);
//     ft_qsort(left, array + size - left, order);
// }