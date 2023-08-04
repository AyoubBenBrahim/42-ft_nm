
#include "ft_nm.h"

void parse_symtab32(t_file *file)
{
    size_t i;
    t_syms *syms = &file->syms;
    ft_memset(syms, 0, sizeof(t_syms));

    for (i = 0; i < file->ehdr32->e_shnum; i++)
    {
        if (file->shdr32[i].sh_type == SHT_SYMTAB)
        {
            syms->n_syms = file->shdr32[i].sh_size / sizeof(Elf32_Sym);
            syms->symb = malloc(syms->n_syms * sizeof(t_symb));

            if (syms->symb == NULL)
            {
                ft_printf("Failed to allocate memory for symbol table\n");
                exit(1);
            }
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

                syms->symb[j].type_char = determin_symb(file, &syms->symb[j]);
                t_symb *symb = &syms->symb[j];

                syms->symb[syms->n_syms_count++] = *symb;
            }
            break;
        }
    }
}