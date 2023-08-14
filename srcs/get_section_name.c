
#include "ft_nm.h"

/*
** u need to test as many files as available, i dont know all the section names..
*/

int get_section_name(uint32_t sh_flag, const char *name)
{
    if (name == NULL)
        return (-1);

// A
    if (!ft_strcmp(name, ".symtab"))
        return 0;
    if (!ft_strcmp(name, ".strtab"))
        return 1;
    if (!ft_strcmp(name, ".shstrtab"))
        return 2;
// B
    if (!ft_strcmp(name, ".bss"))
        return 3;
    if (!ft_strcmp(name, ".tbss"))
        return 4;
    if (!ft_strcmp(name, ".sbss"))
        return 5;
// D
    if (!ft_strcmp(name, ".data"))
        return 6;
    if (!ft_strcmp(name, ".data.rel.ro"))
        return 7;
    if (!ft_strcmp(name, ".data.rel.ro.local"))
        return 8;
    if (!ft_strcmp(name, ".idata"))
        return 9;
    if (!ft_strcmp(name, ".sdata"))
        return 10;
    if (!ft_strcmp(name, ".sdata2"))
        return 11;
    if (!ft_strcmp(name, ".got.plt") || !ft_strcmp(name, ".got"))
        return 12;
    if (!ft_strcmp(name, ".data"))
        return 13;
    if (!ft_strcmp(name, ".debug_macro") || !ft_strcmp(name, ".dtors"))
        return 19;
    if (!ft_strcmp(name, ".dynamic") || !ft_strcmp(name, ".ctors"))
        return 20;
    if (!ft_strcmp(name, ".jcr"))
        return 20;
// N
    if (!ft_strcmp(name, ".comment"))
        return 21;
    if (!ft_strcmp(name, ".copyright"))
        return 22;
    if (!ft_strcmp(name, ".gnu_debug") || !ft_strcmp(name, ".gnu.warning.pthread_attr_getstackaddr") || !ft_strcmp(name, ".gnu.warning.pthread_attr_setstackaddr"))
        return 23;
    if (!ft_strcmp(name, ".ident") || !ft_strcmp(name, ".note.GNU-stack"))
        return 24;
    if (!ft_strcmp(name, ".SUNW") || !ft_strcmp(name, ".SUNW_ctf") || !ft_strcmp(name, ".SUNW_signature"))
        return 25;
    if (!ft_strcmp(name, ".debug_"))
        return 26;
    if (!ft_strcmp(name, ".debug_abbrev"))
        return 26;
    if (!ft_strcmp(name, ".debug_aranges"))
        return 26;
    if (!ft_strcmp(name, ".debug_frame"))
        return 26;
    if (!ft_strcmp(name, ".debug_info"))
        return 26;
    if (!ft_strcmp(name, ".debug_line"))
        return 26;
    if (!ft_strcmp(name, ".debug_loc"))
        return 26;
    if (!ft_strcmp(name, ".debug_pubnames"))
        return 26;
    if (!ft_strcmp(name, ".debug_ranges"))
        return 26;
    if (!ft_strcmp(name, ".debug_str"))
        return 26;
    if (!ft_strcmp(name, ".gnu_debuglink"))
        return 27;

// R
    if (!ft_strcmp(name, ".dynstr") || !ft_strcmp(name, ".interp"))
        return 28;
    if (!ft_strcmp(name, ".rela.plt") || !ft_strcmp(name, ".note.ABI-tag"))
        return 29;
    if (!ft_strcmp(name, ".rel.plt") || !ft_strcmp(name, ".note.gnu.build-id"))
        return 30;
    if (!ft_strcmp(name, ".rela.bss"))
        return 31;
    if (!ft_strcmp(name, ".rela.got") || !ft_strcmp(name, ".rela.dyn"))
        return 32;
    if (!ft_strcmp(name, ".gnu.version_r") || !ft_strcmp(name, ".gnu.version") || !ft_strcmp(name, ".gnu.version_d"))
        return 33;
    if (!ft_strcmp(name, ".rodata"))
        return 34;
    if (!ft_strcmp(name, ".gnu.hash") || !ft_strcmp(name, ".hash"))
        return 35;
    if (!ft_strcmp(name, ".rdata") || !ft_strcmp(name, ".gcc_except_table"))
        return 36;
    if (!ft_strcmp(name, ".reloc"))
        return 37;
    if (!ft_strcmp(name, ".rsrc"))
        return 38;
    if (!ft_strcmp(name, ".rel.dyn"))
        return 39;
    if (!ft_strcmp(name, ".note.netbsd.ident") || !ft_strcmp(name, ".note.netbsd.pax"))
        return 40;
    if (!ft_strcmp(name, ".dynsym"))
        return 41;
// T
    if (!ft_strcmp(name, ".text"))
        return 42;
    if (!ft_strcmp(name, ".text.startup"))
        return 45;
    if (!ft_strcmp(name, ".ctors_array"))
        return 46;
    if (!ft_strcmp(name, ".dtors_array"))
        return 47;
    if (!ft_strcmp(name, ".init_array"))
        return 48;
    if (!ft_strcmp(name, ".fini_array"))
        return 49;
    if (!ft_strcmp(name, ".init"))
        return 50;
    if (!ft_strcmp(name, ".plt"))
        return 51;
    if (!ft_strcmp(name, ".plt.got"))
        return 52;
    if (!ft_strcmp(name, ".fini"))
        return 53;

    if (!ft_strcmp(name, ".eh_frame_hdr") || !ft_strcmp(name, ".eh_frame"))
    {
        if (sh_flag & SHF_WRITE)
            return 20; // D
        else
            return 40; // R
    }

    return -1;
}