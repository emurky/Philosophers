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

void	sleep_wrapper(size_t ms)
{
	size_t	current_time;

	current_time = gettime_in_ms();
	while (gettime_in_ms() < current_time + ms)
		usleep(50);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->all->print_mtx);
	printf("%zu %zu %s\n", gettime_in_ms() - philo->all->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->all->print_mtx);
}

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

void	*philo_thread(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == ODD)
		sleep_wrapper(philo->all->time_to_eat);
	print_status(philo, STATUS_FORK);
	while (true)
	{

	}
	return (NULL);
}

bool	philos_init(t_all *all, t_philo **philos)
{
	unsigned int	i;
	// t_philo			*philos;
	pthread_mutex_t	*forks;

	i = 0;
	*philos = malloc(sizeof(t_philo) * all->philo_count);
	forks = malloc(sizeof(pthread_mutex_t) * all->philo_count);
	if (!(*philos) || !forks)
		return (print_error(ERR_MALLOC));
	while (i < all->philo_count)
	{
		if (pthread_mutex_init(&forks[i], NULL))
			return (print_error(ERR_MTX_INIT));
		if (pthread_mutex_init(&(*philos)[i].death_mtx, NULL))
			return (print_error(ERR_MTX_INIT));
		(*philos)[i].left_fork_mtx = &forks[i];
		(*philos)[i].right_fork_mtx = &forks[(i + 1) % all->philo_count];
		(*philos)[i].id = i + 1;
		(*philos)[i].meals = all->meals;
		(*philos)[i].all = all;
		(*philos)[i].dead = false;
		// if (pthread_create(&(*philos)[i].thread, NULL, &philo_thread, &philos[i]))
		// 	return(print_error(ERR_THRD_CRT));
		// if (pthread_detach((*philos)[i].thread))
		// 	return(print_error(ERR_THRD_DTC));
		i++;
	}
	// i = 0;
	// while (i < all->philo_count)
	// {
	// 	if (pthread_mutex_init(&forks[i], NULL))
	// 		return (print_error(ERR_MTX_INIT));
	// 	if (pthread_mutex_init(&(*philos)[i].death_mtx, NULL))
	// 		return (print_error(ERR_MTX_INIT));
	// 	(*philos)[i].left_fork_mtx = &forks[i];
	// 	(*philos)[i].right_fork_mtx = &forks[(i + 1) % all->philo_count];
	// 	(*philos)[i].id = i + 1;
	// 	(*philos)[i].meals = all->meals;
	// 	(*philos)[i].all = all;
	// 	// if (pthread_create(&(*philos)[i].thread, NULL, &philo_thread, &philos[i]))
	// 	// 	return(print_error(ERR_THRD_CRT));
	// 	// if (pthread_detach((*philos)[i].thread))
	// 	// 	return(print_error(ERR_THRD_DTC));
	// 	i++;
	// }
	// all->philos = philos;
	all->forks = forks;

	return (true);
}

bool	clean_philos(t_all *all, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	if (pthread_mutex_destroy(&all->print_mtx))
		return (print_error(ERR_MTX_DSTR));
	while (i < all->philo_count)
	{
		if (pthread_mutex_destroy(&philos[i].death_mtx))
			return (print_error(ERR_MTX_DSTR));
		if (pthread_mutex_destroy(&all->forks[i]))
			return (print_error(ERR_MTX_DSTR));
		i++;
	}
	free(philos);
	free(all->forks);
	return (true);
}

bool	threads_init(t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philos->all->philo_count)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_thread, &philos[i]))
			return(print_error(ERR_THRD_CRT));
		if (pthread_detach(philos[i].thread))
			return(print_error(ERR_THRD_DTC));
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_all			all;
	t_philo			*philos;

	if (!parser(argc, argv, &all) || !philos_init(&all, &philos))
		return (1);

	// for (int i = 0; i < 5; i++) {
	// 	printf("%zu id from main\n", philos[i].id);
	// }

	threads_init(philos);

// ssize_t i =0;
// 	while (i < all.philo_count)
// 	{
// 		if (pthread_create(&philos[i].thread, NULL, &philo_thread, &philos[i]))
// 			return(print_error(ERR_THRD_CRT));
// 		if (pthread_detach(philos[i].thread))
// 			return(print_error(ERR_THRD_DTC));
// 		i++;
// 	}
	// printf("%d %d %d %d %d\n", all.philo_count, all.time_to_die, all.time_to_eat, all.time_to_sleep, all.meals);

	sleep(5);

	for (size_t i = 0; i < all.philo_count; i++) {
		pthread_join(philos[i].thread, NULL);
	}

	if (!clean_philos(&all, philos))
		return (1);

	return (0);
}
