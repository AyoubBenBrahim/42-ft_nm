nm - an abbreviation for "names" or "name list" -  Display the Symbol Table of a Binary Object File

a tool for examining the contents of object files, archives, and shared libraries.


nm /bin/ls
nm: /bin/ls: no symbols

nm /usr/lib/gcc/x86_64-linux-gnu/8/
nm: Warning: '/usr/lib/gcc/x86_64-linux-gnu/8/' is a directory

ELF — Linux manual page
https://man7.org/linux/man-pages/man5/elf.5.html

read alongside with

https://www.ics.uci.edu/~aburtsev/238P/hw/hw3-elf/hw3-elf.html



```
$ nm hello.o
00000000 T main
                U printf
T before “main” signifies it is in the text section and the “U” before printf means it is undefined in this code. The first column gives the value of the symbol.
```

```
hexdump a.out | head -n 1
0000000 457f 464c
```

32 Little-endian: 
7f 45 4c 46 (ELF)

64-bit Little-endian: 
7f 45 4c 46 02 01 01 00 (ELF header)

The four-byte sequence 7f 45 4c 46 corresponds to the ASCII values of the characters '\x7f', 'E', 'L', and 'F', respectively. 


https://youtu.be/nC1U1LJQL8o

https://youtu.be/XH6jDiKxod8

sections vs segments(program)

sections and segments overlap

sections(file view of the binary) = describe the layout of the binary in its file form , while  the seg describe the binary as it looks when it is loaded into memory. 

section headers describe the layout of the sections

text = code

ro data = readOnly data

data = initialized global vars

bss = uninitialized vars 

https://youtu.be/1VnnbpHDBBA + https://youtu.be/L9okXJH5l2Y

https://youtu.be/hoLYnS2jOV8

https://youtu.be/-UBVQPu5tQo

https://youtu.be/E804eTETaQs


mmap

https://youtu.be/rPV6b8BUwxM

https://youtu.be/m7E9piHcfr4

malloc, sbrk, and mmap

https://youtu.be/c7xf5dvUb_Q


https://medium.com/a-42-journey/nm-otool-everything-you-need-to-know-to-build-your-own-7d4fef3d7507