/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lprieure <lprieure@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:15:05 by lprieure          #+#    #+#             */
/*   Updated: 2023/03/05 15:57:42 by lprieure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>

static void	ft_putstr(char *str, int *size)
{
	int	i;

	if (str == NULL)
		str = "(null)";
	i = 0;
	while (str[i])
	{
		*size += write(1, &str[i], 1);
		i++;
	}
}

static void	ft_putnbr_base(long int n, int base, int *size)
{
	char	base_hex[] = "0123456789abcdef";

	if (n < 0)
	{
		n *= -1;
		*size += write(1, "-", 1);
	}
	if (n >= base)
		ft_putnbr_base((n / base), base, size);
	*size += write(1, &base_hex[n % base], 1);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		i;
	int		size;

	if (format == NULL)
		return (-1);
	va_start(ap, format);
	i = 0;
	size = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1] != '\0')
		{
			if (format[i + 1] == 's')
				ft_putstr(va_arg(ap, char *), &size);
			else if (format[i + 1] == 'd')
				ft_putnbr_base((long int)va_arg(ap, int), 10, &size);
			else if (format[i + 1] == 'x')
				ft_putnbr_base((long int)va_arg(ap, unsigned int), 16, &size);
			i++;
		}
		else
			size += write(1, &format[i], 1);
		i++;
	}
	va_end(ap);
	return (size);
}
