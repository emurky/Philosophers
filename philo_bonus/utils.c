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
	printf("%zu %zu %s\n", gettime_in_ms()
		- philo->all->start_time, philo->id, status);
	if (*status != 'd')
		sem_post(philo->all->print_sem);
}

bool	print_error(char *err_str, t_philo *philos)
{
	free(philos);
	write(2, "Error: ", 7);
	write(2, err_str, ft_strlen(err_str));
	return (false);
}

void	create_semaphore_name(t_philo *philo)
{
	size_t		i;
	size_t		philo_id;
	char		*name;
	const char	*prefix = "philo_";

	philo_id = philo->id;
	name = (char *)philo->death_sem_name;
	i = 0;
	while (prefix[i])
	{
		name[i] = prefix[i];
		i++;
	}
	while (philo_id > 0)
	{
		name[i] = (philo_id % 10) + '0';
		philo_id /= 10;
		i++;
	}
	name[i] = '\0';
}
