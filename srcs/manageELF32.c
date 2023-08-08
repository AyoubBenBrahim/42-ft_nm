
#include "ft_nm.h"

/*
** SOME NOTES:

** Symbol Table (symtab): This section contains information about the symbols defined and used in the ELF file.
** Symbols can represent functions, variables, or other program elements.
** The symbol table provides details such as the symbol s name, type, size, and location.

** String Table: The string table section contains a collection of null-terminated strings.
** It is used to store various string values referenced by other sections,
** such as symbol names, section names, and other string-based information.

** sh_link: field in a section header refers to the index of the associated string table for a given section.
** In the case of the symbol table, sh_link stores the index of the string table section
** containing the symbol names referenced by the symbol table entries.

** By using sh_link, you can retrieve the associated string table section,
** which allows you to access the string names related to the symbols defined in the symbol table.

** the sh_link does not always point to a string table.
** The purpose of the sh_link field is to specify the section index of an associated section,
** which can vary depending on the type of section.

** In some cases, sh_link does point to a string table section.
** For example, in the symbol table section (symtab),
** the sh_link field typically holds the index of the associated string table section
** that contains the symbol names referenced by the symbol table entries. This allows you to retrieve the string names associated with the symbols.

** However, it's important to note that not all sections with a sh_link field will point to a string table.
** The meaning and usage of the sh_link field can differ depending on the section type.
** It is specific to the particular section and its purpose within the ELF file format.

** For instance, in other types of sections like relocation sections (rel or rela),
** the sh_link field may point to a different section that provides additional information about the relocations.
*/

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

            /*
            *** file->shdr32[i] retrieves the section header at index i.
            *** file->shdr32[i].sh_link is a field in the section header that contains the index of the associated string table section.
            *** file->shdr32[file->shdr32[i].sh_link].sh_offset retrieves the offset of the associated string table section within the file.
            *** (file->data + file->shdr32[file->shdr32[i].sh_link].sh_offset) :
            *** adds the offset to the base address of the file's data, resulting in the address of the string table.
            */
            char *strtab = (char *)(file->data + file->shdr32[file->shdr32[i].sh_link].sh_offset);
            syms->strtab = strtab;
            syms->strtab_size = file->shdr32[file->shdr32[i].sh_link].sh_size;
            syms->symtab = file->data + file->shdr32[i].sh_offset;
            syms->symtab_size = file->shdr32[i].sh_size;
            Elf32_Sym *symtab = (Elf32_Sym *)syms->symtab;

            for (size_t j = 0; j < syms->n_syms; j++)
            {
                /*
                 retrieve the offset within the string table (strtab).
                 Adding this offset to strtab gives the address of the symbol's name.
                */
                syms->symb[j].name = strtab + symtab[j].st_name;
                syms->symb[j].type = ELF32_ST_TYPE(symtab[j].st_info);
                syms->symb[j].binding = ELF32_ST_BIND(symtab[j].st_info);
                syms->symb[j].visibility = ELF32_ST_VISIBILITY(symtab[j].st_other);

                syms->symb[j].size = symtab[j].st_size;
                syms->symb[j].value = symtab[j].st_value;
                syms->symb[j].section_tab_index = symtab[j].st_shndx;

                syms->symb[j].type_char = determine_symb(file, &syms->symb[j]);
                t_symb *symb = &syms->symb[j];

                syms->symb[syms->n_syms_count++] = *symb;
            }
            break;
        }
    }
}