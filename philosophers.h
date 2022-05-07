/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtheobal <vtheobal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 12:41:52 by vtheobal          #+#    #+#             */
/*   Updated: 2021/10/31 16:52:30 by vtheobal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int					cnt_ph;
	int					tm_die;
	int					tm_eat;
	int					tm_sleep;
	int					must_eat;
	int					counter;
	t_philo				*ph;
	long int			start_time;
	pthread_mutex_t		write_mutex;
}						t_data;

typedef struct s_philo
{
	int					ph_id;
	int					dead_flag;
	int					eat_tm;
	int					slp_tm;
	int					cnt_eat;
	long int			life;
	t_data				*das;
	pthread_t			thread;
	pthread_mutex_t		l_f;
	pthread_mutex_t		*r_f;
}						t_philo;

long int	time_get(void);
void		ft_sleep(int time);
void		eating(t_philo *ph);
int			ft_error(char *str);
void		forking(t_philo *ph);
void		sleeping(t_philo *ph);
int			check_args(char *str);
void		ft_death(t_data *data);
int			ft_atoi(const char *str);
void		ft_putstr_fd(char *s, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_print(char *str, t_philo *ph);

#endif
