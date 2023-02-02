/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtheobal <vtheobal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 13:00:02 by vtheobal          #+#    #+#             */
/*   Updated: 2021/10/31 17:46:15 by vtheobal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	init_philo(t_data *data, int ind)
{
	pthread_mutex_init(&data->ph[ind].l_f, NULL);
	data->ph[ind].das = data;
	data->ph[ind].eat_tm = data->tm_eat;
	data->ph[ind].slp_tm = data->tm_sleep;
	data->ph[ind].ph_id = ind + 1;
	data->ph[ind].cnt_eat = 0;
	data->ph[ind].dead_flag = 0;
	data->ph[ind].life = time_get();
	data->ph[ind].r_f = NULL;
	if (ind == data->cnt_ph - 1)
		data->ph[ind].r_f = &data->ph[0].l_f;
	else
		data->ph[ind].r_f = &data->ph[ind + 1].l_f;
}

void	*philosophing(void	*das)
{
	t_philo	*ph;

	ph = (t_philo *)das;
	if (ph->ph_id % 2 == 0)
		ft_sleep(ph->eat_tm / 2);
	ph->life = time_get();
	while (1)
	{
		forking(ph);
		eating(ph);
		sleeping(ph);
		ft_print("is thinking", ph);
	}
	return (NULL);
}

void	philo_make(t_data *data)
{
	int		i;

	i = 0;
	data->ph = malloc(sizeof(t_philo) * data->cnt_ph);
	pthread_mutex_init(&data->write_mutex, NULL);
	while (i < data->cnt_ph)
	{
		init_philo(data, i);
		pthread_create(&data->ph[i].thread, NULL, philosophing, &data->ph[i]);
		i++;
	}
	ft_death(data);
}

int	args_pars(int argc, char **argv, t_data *data)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if ((check_args(argv[i])) != 0)
			return (-1);
		i++;
	}
	data->cnt_ph = ft_atoi(argv[1]);
	data->tm_die = ft_atoi(argv[2]);
	data->tm_eat = ft_atoi(argv[3]);
	data->tm_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc >= 5 && argc <= 6)
	{
		data = malloc(sizeof(t_data));
		data->start_time = time_get();
		if (!data)
			return (ft_error("Error Malloc\n"));
		if (args_pars(argc, argv, data))
			return (ft_error("Error parameters\n"));
		philo_make(data);
		free(data);
	}
	else
		return (ft_error("Error number of arguments\n"));
	return (0);
}
