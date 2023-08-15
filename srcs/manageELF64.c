#include "ft_nm.h"

void parse_symtab64(t_file *file)
{
    size_t i;
    t_syms *syms = &file->syms;
    ft_memset(syms, 0, sizeof(t_syms));

    const char *string_table = (char *)(file->data + file->shdr64[file->ehdr64->e_shstrndx].sh_offset);

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
                
                syms->symb[j].name = "";
                syms->symb[j].name = strtab + symtab[j].st_name;
                syms->symb[j].type = ELF64_ST_TYPE(symtab[j].st_info);
                syms->symb[j].binding = ELF64_ST_BIND(symtab[j].st_info);
                syms->symb[j].visibility = ELF64_ST_VISIBILITY(symtab[j].st_other);

                syms->symb[j].size = symtab[j].st_size;
                syms->symb[j].value = symtab[j].st_value;
                syms->symb[j].section_tab_index = symtab[j].st_shndx;

                syms->symb[j].sh_type = SHT_SYMTAB;

                if (ft_strcmp("", syms->symb[j].name) == 0 && symtab[j].st_shndx < file->ehdr64->e_shnum)
                {
                    syms->symb[j].name = (char *)(string_table + file->shdr64[symtab[j].st_shndx].sh_name);
                    syms->symb[j].sh_type = -1; // apart from -a, i dont want section names to be printed
                }

                syms->symb[j].type_char = determine_symb(file, &syms->symb[j]);

                t_symb *symb = &syms->symb[j];
                syms->symb[syms->n_syms_count++] = *symb;
            }
            break;
        }
    }
}
