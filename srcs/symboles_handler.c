#include "ft_nm.h"

/*
*** Function to determine the character corresponding to symbol section
*** The & operator is used to check if a certain flag is set in the section flags,
*** while the | operator is used to check if a combination of flags is set.
*/

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
            c = 'B'; // Section with allocated but zero-initialized or uninitialized / bss
        break;
    case SHT_PROGBITS:
        if (sh_flags & SHF_ALLOC)
        {
            if (sh_flags & SHF_WRITE)
                c = 'D'; // Section with allocated data and write permissions
            else if (sh_flags & SHF_EXECINSTR)
                c = 'T'; // Section with executable instructions
            else
                c = 'R'; // Section with allocated read-only data
        }
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
        c = 'D'; // Section with initialized data
        break;
    default:
        c = '?'; // Unknown section type
        break;
    }

    if (sh_flags == SHT_NULL)
        c = 'N'; // Section with no data in file
    return c;
}

char get_symb_type_and_visibility(t_symb *symb)
{
    uint32_t shndx = symb->section_tab_index; // t_shndx
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
    else if (binding == STB_WEAK && visibility == STV_DEFAULT)
    {
        c = 'W';
        if (shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (shndx == SHN_ABS)
        c = 'A';
    else if (type == STT_GNU_IFUNC)
        c = 'i';
    else if (shndx == SHN_UNDEF)
        c = 'U';
    else if (shndx == SHN_COMMON)
        c = 'C';

    return c;
}

char get_symb_based_on_section(t_file *file, uint16_t section, const char *name)
{

    uint32_t sh_flag = (file->is64) ? file->shdr64[section].sh_flags : file->shdr32[section].sh_flags;
    int ret = get_section_name(sh_flag, name);

    if (is_in_range(ret, 0, 2))
        return 'A';
    else if (is_in_range(ret, 3, 5))
        return 'B';
    else if (is_in_range(ret, 6, 20))
        return 'D';
    else if (is_in_range(ret, 21, 27))
        return 'N';
    else if (is_in_range(ret, 28, 41))
        return 'R';
    else if (is_in_range(ret, 42, 53))
        return 'T';

    return '?';
}

// this section names have the symbole type 'N' but uppercase
bool isNameMatch(const char *name)
{
    char *strings[] = {
        ".debug_abbrev",
        ".debug_aranges",
        ".debug_frame",
        ".debug_info",
        ".debug_line",
        ".debug_loc",
        ".debug_pubnames",
        ".debug_ranges",
        ".debug_str"};
    int numStrings = sizeof(strings) / sizeof(strings[0]);
    for (int i = 0; i < numStrings; i++)
    {
        if (ft_strcmp(name, strings[i]) == 0)
        {
            return true;
        }
    }

    return false;
}

char determine_symb(t_file *file, t_symb *symb)
{
    char c = get_symb_type_and_visibility(symb);
    if (c == '?')
        c = get_symbol_section_char(symb->section_tab_index, file);

    if (symb->name[0] == '.' && symb->sh_type != SHT_SYMTAB)
        c = get_symb_based_on_section(file, symb->section_tab_index, symb->name);

    if (c == '?')
        c = 'T';

    return ((symb->binding == STB_LOCAL && !isNameMatch(symb->name)) ? TO_LOWER(c) : c);
}