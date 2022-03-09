/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:24:12 by emurky            #+#    #+#             */
/*   Updated: 2022/03/07 12:39:16 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_philos(t_all *all, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < all->philo_count)
	{
		sem_close(philos[i].death_sem);
		sem_unlink((char *)philos[i].death_sem_name);
		i++;
	}
	sem_close(all->print_sem);
	sem_unlink("print_sem");
	sem_close(all->forks);
	sem_unlink("forks");
	free(philos);
	return ;
}

size_t	gettime_in_ms(void)
{
	struct timeval	s_time;
	long			current_time_in_milliseconds;

	gettimeofday(&s_time, NULL);
	current_time_in_milliseconds = s_time.tv_sec * 1000 + s_time.tv_usec / 1000;
	return (current_time_in_milliseconds);
}

void	usleep_wrapper(size_t ms)
{
	size_t	current_time;

	current_time = gettime_in_ms();
	while (gettime_in_ms() < current_time + ms)
		usleep(500);
}

void	print_status(t_philo *philo, char *status)
{
	sem_wait(philo->all->print_sem);
	// if (philo->all->finish == false)
	// {
		printf("%zu %zu %s\n", gettime_in_ms() - philo->all->start_time, philo->id, status);
	// }
	if (*status != 'd')
		sem_post(philo->all->print_sem);
}

bool	print_error(char *err_str, t_philo *philos)
{
	free(philos);
	// free(forks);
	write(2, "Error: ", 7);
	write(2, err_str, ft_strlen(err_str));
	return (false);
}

size_t	ft_nbrlen(long int n)
{
	size_t	len;

	len = 1;
	n /= 10;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}
