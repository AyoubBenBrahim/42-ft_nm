
#include "ft_nm.h"

char get_symbol_section_char(uint16_t section, t_file *file)
{
    uint32_t sh_type;
    uint32_t sh_flags;

    if (file->is64)
    {
        sh_type = file->shdr64[section].sh_type;
        sh_flags = file->shdr64[section].sh_flags;
    }
    else
    {
        sh_type = file->shdr32[section].sh_type;
        sh_flags = file->shdr32[section].sh_flags;
    }

    char c = '?';

    switch (sh_type)
    {
    case SHT_NOBITS:
        if (sh_flags & (SHF_ALLOC | SHF_WRITE))
        {
            if (sh_flags & SHF_IA_64_SHORT)
                c = 'S';
            else
                c = 'B';
        }
        break;
    case SHT_IA_64_UNWIND:
        c = 'p';
        break;
    case SHT_PROGBITS:
        if (sh_flags & SHF_ALLOC)
        {
            if (sh_flags & SHF_WRITE)
            {
                if (sh_flags & SHF_IA_64_SHORT)
                    c = 'G';
                else
                    c = 'D';
            }
            else if (sh_flags & SHF_EXECINSTR)
                c = 'T';
            else
            {
                if (sh_type == SHT_NOBITS)
                    c = 'n';
                else
                    c = 'R';
            }
        }
        break;
    case SHT_NULL:
        c = 'N';
        break;
    case SHT_RELA:
    case SHT_REL:
    case SHT_HASH:
    case SHT_GNU_versym:
    case SHT_GNU_verdef:
    case SHT_STRTAB:
    case SHT_DYNSYM:
    case SHT_NOTE:
        if (sh_flags & SHF_ALLOC)
            c = 'R';
        break;
    case SHT_DYNAMIC:
    case SHT_FINI_ARRAY:
    case SHT_INIT_ARRAY:
        c = 'D';
        break;
    case SHT_MIPS_SYMBOL_LIB:
        c = 'T';
        break;
    case SHT_GROUP:
        c = 'N';
        break;
    case SHT_GNU_verneed:
        c = 'R';
        break;
    default:
        c = '?';
        break;
    }
    return c;
}

char get_symb_type_and_visibility(t_symb *symb)
{
    uint16_t shndx = symb->section_tab_index; // t_shndx
    // uint32_t type = ELF32_ST_TYPE(symb->type);    // THIS IS THE STUPIDEST THING I'VE EVER DONE
    uint32_t type = symb->type;
    uint8_t binding = symb->binding;
    uint8_t visibility = symb->visibility;

    char c = '?';
    if (binding == STB_GNU_UNIQUE)
        c = 'u';
    else if (binding == STB_WEAK && type == STT_OBJECT)
    {
        c = 'V';
        if (shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (binding == STB_WEAK && visibility == STV_DEFAULT) //(binding == STB_WEAK)
    {
        c = 'W';
        if (shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (shndx == SHN_UNDEF)
        c = 'U';
    else if (shndx == SHN_ABS)
        c = 'A';
    else if (shndx == SHN_COMMON)
        c = 'C';
    else if (type == STT_GNU_IFUNC)
        c = 'i';

    return c;
}

char determin_symb(t_file *file, t_symb *symb)
{
    char c = get_symb_type_and_visibility(symb);
    if (c == '?')
        c = get_symbol_section_char(symb->section_tab_index, file);
    if (symb->binding == STB_LOCAL)
        c = TO_LOWER(c);
    return c;
}

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

            // printf("syms->n_syms = %ld\n", syms->n_syms); // SEGV when deleting this line or when using ft_printf

            syms->symb = malloc(syms->n_syms * sizeof(t_symb));
            // syms->symb = (t_symb *)malloc(syms->n_syms * sizeof(t_symb));

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

                // char c = determin_symb(file, &syms->&syms->ymb[j]);
                syms->symb[j].type_char = determin_symb(file, &syms->symb[j]);
                t_symb *symb = &syms->symb[j];

                syms->symb[syms->n_syms_count++] = *symb;
                // store_symb(&file, syms->symb[j].name, syms->symb[j].value, c);
            }
            break;
        }
    }
}