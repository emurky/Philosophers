#include "philo.h"

#define PH_COUNT	500

long	gettime_in_ms(void)
{
	struct timeval		s_time;
	long				current_time_in_milliseconds;

	if (gettimeofday(&s_time, NULL))
	{
		return (-1);
	}
	current_time_in_milliseconds = s_time.tv_sec * 1000 + s_time.tv_usec / 1000;
	return (current_time_in_milliseconds);
}

void	*test(void *arg)
{
	t_philo	*ph = arg;
	// int		count = 0;

	for (int i = 0; i < 100000; i++) {
		// count++;
		pthread_mutex_lock(&ph->mutex);
		ph->sum++;
		pthread_mutex_unlock(&ph->mutex);
	}
	// pthread_mutex_lock(&ph->mutex);
	// ph->sum += count;
	// pthread_mutex_unlock(&ph->mutex);
	return (NULL);
}

int	main(void)
{
	pthread_t		philos[PH_COUNT];
	t_philo			ph;
	int				i = 0;
	long long		time = 0;
	
	pthread_mutex_init(&ph.mutex, NULL);
	ph.sum = 0;
	while (i < PH_COUNT)
	{
		if (pthread_create(&philos[i], NULL, &test, &ph))
			return (1);
		i++;
	}
	i = 0;
	while (i < PH_COUNT)
	{
		if (pthread_join(philos[i], NULL))
			return (1);
		i++;
	}
	printf("hi from philo, sum is %d\n", ph.sum);
	pthread_mutex_destroy(&ph.mutex);

	return (0);
}
