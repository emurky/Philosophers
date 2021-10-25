#include "philo.h"

#define PH_COUNT	500

size_t	gettime_in_ms(void)
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

// void	*test(void *arg)
// {
// 	t_philo	*ph = arg;
// 	// int		count = 0;

// 	for (int i = 0; i < 100000; i++) {
// 		// count++;
// 		pthread_mutex_lock(&ph->mutex);
// 		ph->sum++;
// 		pthread_mutex_unlock(&ph->mutex);
// 	}
// 	// pthread_mutex_lock(&ph->mutex);
// 	// ph->sum += count;
// 	// pthread_mutex_unlock(&ph->mutex);
// 	return (NULL);
// }

bool	parser(int argc, char **argv, t_all *args)
{
	char	**ptr_argv;

	ptr_argv = argv;
	args->meals = -1;
	if (argc != 5 && argc != 6)// && argc != 1)
		return (print_error(ERR_ARGS));
	ptr_argv++;
	while (*ptr_argv)
	{
		if (!str_isnumeric(*ptr_argv))
			return (print_error(ERR_NUM));
		ptr_argv++;
	}
	args->philo_count = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		args->meals = ft_atoi(argv[5]);
	args->start_time = gettime_in_ms();
	if (pthread_mutex_init(&args->print_mtx, NULL))
		return (print_error(ERR_MTX_INIT));
	return (true);
}

bool	start_threading(t_all *all)
{
	unsigned int	i;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	i = 0;
	philos = malloc(sizeof(t_philo) * all->philo_count);
	forks = malloc(sizeof(pthread_mutex_t) * all->philo_count);
	if (!philos || !forks)
		return (print_error(ERR_MALLOC));
	while (i < all->philo_count)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			return (print_error(ERR_MTX_INIT));
		if (pthread_mutex_init(&philos[i].death_mtx, NULL))
			return (print_error(ERR_MTX_INIT));
		philos[i].left_fork_mtx = &forks[i];
		philos[i].right_fork_mtx = &forks[(i + 1) % all->philo_count];
		philos[i].id = i + 1;
		philos[i].meals = all->meals;
		i++;
	}
	all->philos = philos;
	all->forks = forks;
	return (true);
}

bool	clean_philos(t_all *all)
{
	unsigned int	i;

	i = 0;
	if (pthread_mutex_destroy(&all->print_mtx))
		return (print_error(ERR_MTX_DSTR));
	while (i < all->philo_count)
	{
		if (pthread_mutex_destroy(&all->philos[i].death_mtx))
			return (print_error(ERR_MTX_DSTR));
		if (pthread_mutex_destroy(&all->forks[i]))
			return (print_error(ERR_MTX_DSTR));
		i++;
	}
	free(all->philos);
	free(all->forks);
	return (true);
}

int	main(int argc, char **argv)
{
	// pthread_t		philos[PH_COUNT];
	// t_philo			ph;
	// int				i = 0;

	t_all			all;

	if (!parser(argc, argv, &all) || !start_threading(&all))
	{
		return (1);
	}
	

	printf("%d %d %d %d %d\n", all.philo_count, all.time_to_die, all.time_to_eat, all.time_to_sleep, all.meals);
	// pthread_mutex_init(&ph.mutex, NULL);
	// ph.sum = 0;
	// while (i < PH_COUNT)
	// {
	// 	if (pthread_create(&philos[i], NULL, &test, &ph))
	// 		return (1);
	// 	i++;
	// }
	// i = 0;
	// while (i < PH_COUNT)
	// {
	// 	if (pthread_join(philos[i], NULL))
	// 		return (1);
	// 	i++;
	// }
	// printf("hi from philo, sum is %d\n", ph.sum);
	// pthread_mutex_destroy(&ph.mutex);

	if (!clean_philos(&all))
		return (1);

	return (0);
}
