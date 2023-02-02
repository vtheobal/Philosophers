/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtheobal <vtheobal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 14:19:13 by vtheobal          #+#    #+#             */
/*   Updated: 2021/10/31 16:57:23 by vtheobal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	ft_print(char *str, t_philo *ph)
{
	long int	time;

	time = time_get() - ph->das->start_time;
	pthread_mutex_lock(&ph->das->write_mutex);
	printf("%ld Philo %d %s\n", time, ph->ph_id, str);
	if (!ph->dead_flag)
		pthread_mutex_unlock(&ph->das->write_mutex);
}

void	ft_sleep(int time)
{
	long int	start;

	start = time_get();
	while ((time_get() - start) < (long int)time)
		usleep(time / 10);
}

void	forking(t_philo *ph)
{
	pthread_mutex_lock(&ph->l_f);
	ft_print("has taken fork", ph);
	pthread_mutex_lock(ph->r_f);
	ft_print("has taken fork", ph);
}

void	eating(t_philo *ph)
{
	ft_print("is sleeping", ph);
	ph->cnt_eat++;
	ph->life = time_get();
	ft_sleep(ph->eat_tm);
	pthread_mutex_unlock(&ph->l_f);
	pthread_mutex_unlock(ph->r_f);
}

void	sleeping(t_philo *ph)
{
	ft_print("is sleeping", ph);
	ft_sleep(ph->slp_tm);
}
