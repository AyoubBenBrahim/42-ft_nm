/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayb***** <ayb*****@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 15:08:03 by ayb*****          #+#    #+#             */
/*   Updated: 2020/01/25 20:17:18 by ayb*****         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
// # include <linux/elf.h>
#include "../inc/elf.h"


#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

# define READ_LINE			get_next_line(0, &line)
# define FREE_LINE			ft_strdel(&line)
# define GNL_RET_CHCK		0
# define TO_LOWER(x)		(x | ' ')

typedef enum				e_boolean
{
	FALSE,
	TRUE,
}							t_boolean;

typedef struct				s_store
{
	int						row;
	int						col;
	int						score;
}							t_store;







// typedef struct {
// 	const char *path;
// 	int fd;
// 	struct stat stat;
// 	void* data;
// 	size_t data_size;
// 	t_list *symbols;
// 	int	elfclass;

// 	Elf64_Shdr *shdr64;
// 	Elf64_Ehdr *ehdr64;

// 	Elf32_Shdr *shdr32;
// 	Elf32_Ehdr *ehdr32;
// } t_file;

// typedef struct	s_file {
// 	void			*start;
// 	void			*end;
// 	t_file_error	error;
// 	char const		*filename;
// 	char const		*virtualname;
// 	uint64_t		filesize;
// 	t_bool			swap_bits;
// 	t_file_type		virtualtype;
// 	t_arch			arch;
// 	uint64_t		nsects;
// 	t_list			*mysects;
// 	t_list			*mysyms;
// }				t_file;

// struct	nm_file{
// 	union u_elf_hdr	header;

// 	unsigned char	arch;
// 	off_t		offset;
// 	struct stat	stat;
// 	void		*mem;
// 	t_list_head	symbols;
// };

#endif
