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

void	launch_process(t_philo *philo)
{
	pthread_t	thread;

	philo->last_eating_time = gettime_in_ms();
	pthread_create(&thread, NULL, philo_thread, philo);
	pthread_detach(thread);

	size_t		i;
	bool		all_meals_eaten;
	t_all		*all;

	all = philo->all;

	while (true)
	{
		i = 0;
		all_meals_eaten = true;
		// while (i < all->philo_count)
		// {
			// pthread_mutex_lock(&philos[i].death_mtx);
			sem_wait(all->death_sem);
			if (gettime_in_ms() - philo->last_eating_time
				> (size_t)all->time_to_die)
				return (is_dead(philo, all));
			// pthread_mutex_lock(&all->finish_mtx);
			sem_wait(all->finish_sem);
			if (philo->meals < all->meals)
				all_meals_eaten = false;
			// pthread_mutex_unlock(&all->finish_mtx);
			sem_post(all->finish_sem);
			// pthread_mutex_unlock(&philos[i].death_mtx);
			sem_post(all->death_sem);
			i++;
		// }
		// pthread_mutex_lock(&all->finish_mtx);
		sem_wait(all->finish_sem);
		if (all->finish == true || (all_meals_eaten == true && all->meals >= 0))
		{
			sem_post(all->finish_sem);
			return /*(void)pthread_mutex_unlock(&all->finish_mtx)*/;
		}
		sem_post(all->finish_sem);
		// pthread_mutex_unlock(&all->finish_mtx);
	}


}

bool	threads_init(t_philo *philos, t_all *all)
{
	size_t	i;

	i = 0;
	while (i < all->philo_count)
	{
		// if (pthread_create(&philos[i].thread, NULL, &philo_thread, &philos[i]))
		// 	return (print_error(ERR_THRD_CRT, philos, all->forks));
		philos[i].pid = fork();
		if (philos[i].pid == 0)
		{
			launch_process(&philos[i]);
		}
		i++;
	}
threads_finish(philos, all);
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
		// pthread_mutex_lock(&philo->all->finish_mtx);
		sem_wait(philo->all->finish_sem);
		if (philo->all->finish == true)
		{
			// pthread_mutex_unlock(&philo->all->finish_mtx);
			sem_post(philo->all->finish_sem);
			break ;
		}
		// pthread_mutex_unlock(&philo->all->finish_mtx);
		sem_post(philo->all->finish_sem);
		if (!eating_time(philo))
			break ;
		sleeping_and_maybe_thinking(philo);
	}
	return (NULL);
}

bool	eating_time(t_philo *philo)
{
	// pthread_mutex_lock(&philo->death_mtx);
	sem_wait(philo->all->death_sem);
	if (philo->meals == philo->all->meals)
	{
		sem_post(philo->all->death_sem);
		return (false);
	}
	sem_post(philo->all->death_sem);
	// pthread_mutex_lock(philo->left_fork_mtx);
	sem_wait(philo->all->forks);
	print_status(philo, STATUS_FORK);
	// pthread_mutex_lock(philo->right_fork_mtx);
	sem_wait(philo->all->forks);
	print_status(philo, STATUS_FORK);
	// pthread_mutex_lock(&philo->death_mtx);
	sem_wait(philo->all->death_sem);
	philo->last_eating_time = gettime_in_ms();
	philo->meals++;
	// pthread_mutex_unlock(&philo->death_mtx);
	sem_post(philo->all->death_sem);
	print_status(philo, STATUS_EAT);
	usleep_wrapper(philo->all->time_to_eat);
	return (true);
}

void	sleeping_and_maybe_thinking(t_philo *philo)
{
	print_status(philo, STATUS_SLEEP);
	// pthread_mutex_unlock(philo->right_fork_mtx);
	// pthread_mutex_unlock(philo->left_fork_mtx);
	sem_post(philo->all->forks);
	sem_post(philo->all->forks);
	usleep_wrapper(philo->all->time_to_sleep);
	print_status(philo, STATUS_THINK);
}

bool	threads_finish(t_philo *philos, t_all *all)
{
	size_t	i;
	int		status;

	i = 0;
	status = 0;
	// if (all->philo_count == 1)
		// pthread_mutex_unlock(&philos->all->forks[0]);
		// sem_post(all->forks);
	while (status == 0 && i < all->philo_count)
	{
		// if (pthread_join(philos[i].thread, NULL))
		// 	return (print_error(ERR_THRD_JOIN, philos, all->forks));
		waitpid(-1, &status, 0);
		status = WEXITSTATUS(status);
		i++;
	}
	// if (status == 0)
	// 	sem_wait(all->death_sem);
	i = 0;
	// (void)philos;
	while (i < all->philo_count)
	{
		kill(philos[i].pid, SIGKILL);
		i++;
	}
	return (true);
}
