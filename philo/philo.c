#include "philo.h"

#define PH_COUNT	100

void	*test(void *arg)
{
	t_philo	*ph = arg;

	for (int i = 0; i < 100000; i++) {
		pthread_mutex_lock(&ph->mutex);
		ph->sum++;
		pthread_mutex_unlock(&ph->mutex);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	philos[PH_COUNT];
	t_philo		ph;
	int			i = 0;

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
