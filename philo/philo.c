/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:23:34 by emurky            #+#    #+#             */
/*   Updated: 2021/11/02 03:16:14 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_all		all;
	t_philo		*philos;

	philos = NULL;
	memset(&all, 0, sizeof(all));
	if (argc != 5 && argc != 6)
		return (!print_error(ERR_ARGS, philos, all.forks));
	if (!parser(argv, &all))
	{
		if (!all.meals)
			return (0);
		return (1);
	}
	philos = philos_init(&all);
	if (!philos)
		return (1);
	if (!threads_init(philos, &all))
		return (1);
	check_philos(philos, &all);
	if (!threads_finish(philos, &all))
		return (1);
	if (!clean_philos(&all, philos))
		return (1);
	return (0);
}

bool	parser(char **argv, t_all *args)
{
	args->meals = -1;
	if (!args_are_numeric(argv))
		return (print_error(ERR_NUM, NULL, NULL));
	args->philo_count = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		args->meals = ft_atoi(argv[5]);
	if (args->time_to_die < 0 || args->time_to_eat < 0
		|| args->time_to_sleep < 0 || (args->meals < 0 && argv[5]))
		return (print_error(ERR_INT_OVRFLW, NULL, NULL));
	if (!args->meals)
		return (false);
	if (!args->philo_count)
		return (print_error(ERR_PHILO, NULL, NULL));
	if (args->philo_count > THREADS_LIMIT)
		return (print_error(ERR_THREADS, NULL, NULL));
	if (args->time_to_die < 10 || args->time_to_eat < 10
		|| args->time_to_sleep < 10)
		return (print_error(ERR_TIME, NULL, NULL));
	if (pthread_mutex_init(&args->finish_mtx, NULL))
		return (print_error(ERR_MTX_INIT, NULL, NULL));
	return (true);
}

t_philo	*philos_init(t_all *all)
{
	size_t		i;
	t_philo		*philos;

	i = 0;
	philos = NULL;
	philos = malloc(sizeof(t_philo) * all->philo_count);
	all->forks = malloc(sizeof(pthread_mutex_t) * all->philo_count);
	if (!philos || !all->forks)
		return ((t_philo *)print_error(ERR_MALLOC, philos, all->forks));
	all->start_time = gettime_in_ms();
	while (i < all->philo_count)
	{
		if (pthread_mutex_init(&all->forks[i], NULL)
			|| pthread_mutex_init(&philos[i].death_mtx, NULL))
			return ((t_philo *)print_error(ERR_MTX_INIT, philos, all->forks));
		philos[i].left_fork_mtx = &all->forks[i];
		philos[i].right_fork_mtx = &all->forks[(i + 1) % all->philo_count];
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].all = all;
		philos[i].dead = false;
		philos[i].last_eating_time = gettime_in_ms();
		i++;
	}
	return (philos);
}

void	check_philos(t_philo *philos, t_all *all)
{
	size_t		i;
	bool		all_meals_eaten;

	while (true)
	{
		i = 0;
		all_meals_eaten = true;
		while (i < all->philo_count)
		{
			pthread_mutex_lock(&philos[i].death_mtx);
			if (gettime_in_ms() - philos[i].last_eating_time
				> (size_t)all->time_to_die)
				return (is_dead(&philos[i], all));
			pthread_mutex_lock(&all->finish_mtx);
			if (philos[i].meals < all->meals)
				all_meals_eaten = false;
			pthread_mutex_unlock(&all->finish_mtx);
			pthread_mutex_unlock(&philos[i].death_mtx);
			i++;
		}
		pthread_mutex_lock(&all->finish_mtx);
		if (all->finish == true || (all_meals_eaten == true && all->meals >= 0))
			return ((void)pthread_mutex_unlock(&all->finish_mtx));
		pthread_mutex_unlock(&all->finish_mtx);
	}
}

void	is_dead(t_philo *philo, t_all *all)
{
	philo->dead = true;
	print_status(philo, STATUS_DEATH);
	pthread_mutex_lock(&all->finish_mtx);
	all->finish = true;
	pthread_mutex_unlock(&all->finish_mtx);
	pthread_mutex_unlock(&philo->death_mtx);
}
