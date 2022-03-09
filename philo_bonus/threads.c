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

	// bool		all_meals_eaten;
	t_all		*all;

	all = philo->all;

	while (true)
	{
		// all_meals_eaten = true;
// printf()
		sem_wait(philo->death_sem);
		if (gettime_in_ms() - philo->last_eating_time > (size_t)all->time_to_die)
		{
			print_status(philo, STATUS_DEATH);
			sem_post(philo->death_sem);
			// sem_wait(all->print_sem);
			// printf("dieieeeed\n");
			exit(PHILO_DIED);
		}
		sem_post(philo->death_sem);
		// printf("check\n");
	}
}

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
			launch_process(&philos[i]);
		i++;
	}
// processes_finish(philos, all);
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
		// sem_wait(philo->all->finish_sem);
		// if (philo->all->finish == true)
		// {
		// 	sem_post(philo->all->finish_sem);
		// 	break ;
		// }
		// sem_post(philo->all->finish_sem);
		// if (philo->meals < philo->all->meals)
		// 	exit(1) ;
		eating_time(philo);
		sleeping_and_maybe_thinking(philo);
		// printf("philo_thread\n");
	}
	// exit(1);
	return (NULL);
}

bool	eating_time(t_philo *philo)
{
	if (philo->meals == philo->all->meals)
	{
		// printf("%zu poel v eating time\n", philo->id);
		exit(MEALS_EATEN);
	}
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
	return (true);
}

void	sleeping_and_maybe_thinking(t_philo *philo)
{
	print_status(philo, STATUS_SLEEP);
	sem_post(philo->all->forks);
	sem_post(philo->all->forks);
	usleep_wrapper(philo->all->time_to_sleep);
	print_status(philo, STATUS_THINK);
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
	// printf("nachinaem kill\n");
	// if (status == 0)
	// {
	// 	sem_wait(all->print_sem);
	// }
	i = 0;
	while (i < all->philo_count)
	{
		kill(philos[i].pid, SIGKILL);
		i++;
	}
	return ;
}
