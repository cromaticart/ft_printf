/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_wstr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cammapou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/03 11:38:26 by cammapou          #+#    #+#             */
/*   Updated: 2018/04/03 12:59:27 by cammapou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_put_wstr_c(t_env *op, char c)
{
	op->ret += write(1, &c, 1);
}

int	ft_put_wstr(t_env *op, wchar_t c)
{
	if (c <= 0x7F)
		ft_put_wstr_c(op, c);
	else if (c <= 0x7FF)
	{
		ft_put_wstr_c(op, (c >> 6) + 0xC0);
		ft_put_wstr_c(op, (c & 0x3F) + 0x80);
	}
	else if (c <= 0xFFFF)
	{
		ft_put_wstr_c(op, ((c >> 12) + 0xE0));
		ft_put_wstr_c(op, ((c >> 6) & 0x3F) + 0x80);
		ft_put_wstr_c(op, (c & 0x3F) + 0x80);
	}
	else if (c <= 0x10FFFF)
	{
		ft_put_wstr_c(op, (c >> 18) + 0xF0);
		ft_put_wstr_c(op, ((c >> 12) & 0x3F) + 0x80);
		ft_put_wstr_c(op, ((c >> 6) & 0x3F) + 0x80);
		ft_put_wstr_c(op, (c & 0x3F) + 0x80);
	}
	return (0);
}

int		ft_get_wstr_len(wchar_t *wc)
{
	int i;
	int len;

	i = -1;
	len = 0;
	while (wc[++i] != 0)
	{
		if (wc[i] <= 0x7F)
			len++;
		else if (wc[i] <= 0x7FF)
			len += 2;
		else if (wc[i] <= 0xFFFF)
			len += 3;
		else if (wc[i] <= 0x10FFFF)
			len += 4;
	}
	return (len);
}

void	ft_print_wstr_minus(t_env *op, wchar_t *wc)
{
	int i;

	i = -1;
	if (op->flags.press > 0)
	{
		while (wc[++i] != 0 && i < op->flags.press)
			ft_put_wstr(op, wc[i]);
	}
	else
	{
		while (wc[++i] != 0)
			ft_put_wstr(op, wc[i]);
	}
}

void	ft_print_wstr(t_env *op, wchar_t *wc)
{
	int		i;

	i = -1;
	op->len = ft_get_wstr_len(wc);
	if (op->flags.neg)
		ft_print_wstr_minus(op, wc);
	else
	{
		while (op->flags.width-- > op->len)
		{
			op->ret += (op->flags.zero == 1 ?
					write(1, "0", 1) : write(1, " ", 1));
		}
		if (op->flags.press > 0)
		{
			while (wc[++i] && i < op->flags.press - 1)
				ft_put_wstr(op, wc[i]);
		}
		else
		{
			while (wc[++i] != 0)
				ft_put_wstr(op, wc[i]);
		}
	}
	++op->i;
}
