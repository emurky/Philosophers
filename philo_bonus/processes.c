/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:23:40 by emurky            #+#    #+#             */
/*   Updated: 2022/03/07 12:39:10 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	processes_init(t_philo *philos, t_all *all)
{
	size_t	i;

	all->start_time = gettime_in_ms();
	i = 0;
	while (i < all->philo_count)
	{
		philos[i].pid = fork();
		if (philos[i].pid < 0)
			return (false);
		if (philos[i].pid == 0)
			launch_processes_and_check(&philos[i]);
		i++;
	}
	return (true);
}

void	launch_processes_and_check(t_philo *philo)
{
	pthread_t	thread;

	philo->last_eating_time = gettime_in_ms();
	pthread_create(&thread, NULL, philo_thread, philo);
	pthread_detach(thread);
	while (true)
	{
		sem_wait(philo->death_sem);
		if (gettime_in_ms() - philo->last_eating_time
			> (size_t)philo->all->time_to_die)
		{
			print_status(philo, STATUS_DEATH);
			sem_post(philo->death_sem);
			exit(PHILO_DIED);
		}
		sem_post(philo->death_sem);
	}
}

void	*philo_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (true)
	{
		if (philo->meals == philo->all->meals)
			exit(MEALS_EATEN);
		sem_wait(philo->all->forks);
		print_status(philo, STATUS_FORK);
		sem_wait(philo->all->forks);
		print_status(philo, STATUS_FORK);
		sem_wait(philo->death_sem);
		philo->last_eating_time = gettime_in_ms();
		philo->meals++;
		sem_post(philo->death_sem);
		print_status(philo, STATUS_EAT);
		usleep_wrapper(philo->all->time_to_eat);
		print_status(philo, STATUS_SLEEP);
		sem_post(philo->all->forks);
		sem_post(philo->all->forks);
		usleep_wrapper(philo->all->time_to_sleep);
		print_status(philo, STATUS_THINK);
	}
	return (NULL);
}

void	processes_finish(t_philo *philos, t_all *all)
{
	size_t	i;
	int		status;

	i = 0;
	status = 0;
	while (status == MEALS_EATEN && i < all->philo_count)
	{
		waitpid(-1, &status, 0);
		status = WEXITSTATUS(status);
		i++;
	}
	i = 0;
	while (i < all->philo_count)
	{
		kill(philos[i].pid, SIGKILL);
		i++;
	}
}
