/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_poubx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayb***** <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 16:23:58 by ayb*****          #+#    #+#             */
/*   Updated: 2019/11/24 20:50:55 by akhossan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
**	Prints types p, o, u, b, x/X, corresponding to, respectively,
**	void pointer, octal, unsigned, binary and hexadecimal conversions.
*/

void	ft_print_poubx(t_args *args, t_vars *var)
{
	args->unsign_ed = ft_unsigned(args, var);
	if (args->unsign_ed == 0 && args->precision == 0)
		args->result = ft_strdup("");
	else
		args->result = ft_base(args);
	if ((!args->flag && !args->width && args->precision == -1))
	{
		if (args->spec == 'p')
			args->result = ft_strjoin_free(ft_strdup("0x"), args->result);
		var->counter += ft_putstr(args->result);
	}
	else
	{
		ft_precision(args);
		ft_width(args);
		var->counter += ft_putstr(args->result);
	}
	ft_strdel(&args->result);
}
