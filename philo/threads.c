/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:23:40 by emurky            #+#    #+#             */
/*   Updated: 2021/11/02 03:32:56 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	threads_init(t_philo *philos, t_all *all)
{
	size_t	i;

	i = 0;
	while (i < all->philo_count)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_thread, &philos[i]))
			return (print_error(ERR_THRD_CRT));
		i++;
	}
	return (true);
}

void	*philo_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == ODD)
		usleep_wrapper(philo->all->time_to_eat);
	while (true)
	{
		pthread_mutex_lock(&philo->all->finish_mtx);
		if (philo->all->finish == true)
		{
			pthread_mutex_unlock(&philo->all->finish_mtx);
			break ;
		}
		pthread_mutex_unlock(&philo->all->finish_mtx);
		if (!eating_time(philo))
			break ;
		sleeping_and_maybe_thinking(philo);
	}
	return (NULL);
}

bool	eating_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->death_mtx);
	if (philo->all->meals > 0 && philo->meals == philo->all->meals)
	{
		pthread_mutex_unlock(&philo->death_mtx);
		return (false);
	}
	philo->meals++;
	pthread_mutex_unlock(&philo->death_mtx);
	pthread_mutex_lock(philo->left_fork_mtx);
	print_status(philo, STATUS_FORK);
	pthread_mutex_lock(philo->right_fork_mtx);
	print_status(philo, STATUS_FORK);
	pthread_mutex_lock(&philo->death_mtx);
	philo->last_eating_time = gettime_in_ms();
	pthread_mutex_unlock(&philo->death_mtx);
	print_status(philo, STATUS_EAT);
	usleep_wrapper(philo->all->time_to_eat);
	return (true);
}

void	sleeping_and_maybe_thinking(t_philo *philo)
{
	print_status(philo, STATUS_SLEEP);
	pthread_mutex_unlock(philo->right_fork_mtx);
	pthread_mutex_unlock(philo->left_fork_mtx);
	usleep_wrapper(philo->all->time_to_sleep);
	print_status(philo, STATUS_THINK);
	if (philo->all->philo_count % 2 == ODD)
		usleep_wrapper(philo->all->time_to_eat);
}

bool	threads_finish(t_philo *philos, t_all *all)
{
	size_t	i;

	i = 0;
	if (all->philo_count == 1)
		pthread_mutex_unlock(&philos->all->forks[0]);
	while (i < all->philo_count)
	{
		if (pthread_join(philos[i].thread, NULL))
			return (print_error(ERR_THRD_JOIN));
		i++;
	}
	return (true);
}
