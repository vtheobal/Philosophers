/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtheobal <vtheobal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 13:35:26 by vtheobal          #+#    #+#             */
/*   Updated: 2021/10/31 16:38:44 by vtheobal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	check_args(char *str)
{
	int		i;

	i = 0;
	while (*str != '\0')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (-1);
		str++;
	}
	return (0);
}

int	ft_error(char *str)
{
	ft_putstr_fd(str, 1);
	return (0);
}

int	check_death_dop(t_philo ph)
{
	if (ph.cnt_eat >= ph.das->must_eat && ph.das->must_eat != -1)
		ph.das->counter++;
	if (ph.das->tm_die < time_get() - ph.life)
	{
		ph.dead_flag = 1;
		ft_print("died", &ph);
		return (1);
	}
	return (0);
}

void	*check_death(void	*das)
{
	t_data	*data;
	int		i;

	data = (t_data *)das;
	data->counter = 0;
	i = 0;
	while (1)
	{
		i = 0;
		while (i < data->cnt_ph)
		{
			if (check_death_dop(data->ph[i]))
				return (NULL);
			i++;
		}
		if (data->counter == data->cnt_ph)
		{
			pthread_mutex_lock(&data->write_mutex);
			return (NULL);
		}
	}
	return (NULL);
}

void	ft_death(t_data *data)
{
	pthread_t	death;

	pthread_create(&death, NULL, check_death, data);
	pthread_join(death, NULL);
	pthread_detach(death);
	free(data->ph);
}
