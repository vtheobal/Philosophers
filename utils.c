/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtheobal <vtheobal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 13:39:42 by vtheobal          #+#    #+#             */
/*   Updated: 2021/10/31 16:48:26 by vtheobal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		ft_putchar_fd((char)s[i], fd);
		i++;
	}
}

int	ft_error_message(unsigned long ret, unsigned long max, int negativ)
{
	if (ret > max && negativ == 1)
		return (-1);
	else if (ret > max && negativ == -1)
		return (0);
	return ((long)ret * negativ);
}

int	ft_atoi(const char *str)
{
	int				i;
	int				negativ;
	unsigned long	ret;
	unsigned long	max;

	i = 0;
	negativ = 1;
	ret = 0;
	max = 9223372036854775807;
	while ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n') || \
	(str[i] == '\v') || (str[i] == '\f') || (str[i] == '\r'))
		i++;
	if ((str[i] == '-') || (str[i] == '+'))
	{
		if (str[i] == '-')
			negativ = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		ret *= 10;
		ret += (str[i++] - '0');
	}
	return (ft_error_message(ret, max, negativ));
}

long int	time_get(void)
{
	long int			time;
	struct timeval		current_time;

	time = 0;
	if (gettimeofday(&current_time, NULL) == -1)
		ft_error("Gettimeofday returned -1\n");
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}
