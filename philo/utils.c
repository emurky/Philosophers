#include "philo.h"

bool	clean_philos(t_all *all, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < all->philo_count)
	{
		if (pthread_mutex_destroy(&all->forks[i]))
			return (print_error(ERR_MTX_DSTR));
		if (pthread_mutex_destroy(&philos[i].death_mtx))
			return (print_error(ERR_MTX_DSTR));
		i++;
	}
	if (pthread_mutex_destroy(&all->finish_mtx))
		return (print_error(ERR_MTX_DSTR));
	free(philos);
	free(all->forks);
	return (true);
}

size_t	gettime_in_ms(void)
{
	struct timeval		s_time;
	long				current_time_in_milliseconds;

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
	pthread_mutex_lock(&philo->all->finish_mtx);
	if (philo->all->finish == false)
	{
		printf("%zu %zu %s\n",
			gettime_in_ms() - philo->all->start_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->all->finish_mtx);
}

bool	print_error(char *err_str)
{
	write(2, "Error: ", 7);
	write(2, err_str, ft_strlen(err_str));
	return (false);
}
