#include "philo.h"

size_t	gettime_in_ms(void)
{
	struct timeval		s_time;
	long				current_time_in_milliseconds;

	gettimeofday(&s_time, NULL);
	// if (gettimeofday(&s_time, NULL))
	// {
	// 	return (-1);
	// }
	current_time_in_milliseconds = s_time.tv_sec * 1000 + s_time.tv_usec / 1000;
	return (current_time_in_milliseconds);
}

void	usleep_wrapper(size_t ms)
{
	size_t	current_time;

	current_time = gettime_in_ms();
	while (gettime_in_ms() < current_time + ms)
		usleep(500);
	// usleep(1000 * ms);
}

void	print_status(t_philo *philo, char *status)
{
	// pthread_mutex_lock(&philo->all->print_mtx);
	pthread_mutex_lock(&philo->all->finish_mtx);
	if (philo->all->finish == false)
	{
		printf("%zu %zu %s\n",
			gettime_in_ms() - philo->all->start_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->all->finish_mtx);
	// pthread_mutex_unlock(&philo->all->print_mtx);
}

bool	parser(int argc, char **argv, t_all *args)
{
	char	**ptr_argv;

	ptr_argv = argv;
	args->meals = -1;
	if (argc != 5 && argc != 6)
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
	if (!args->meals)
		return (false);
	if (!args->philo_count)
		return(print_error(ERR_PHILO));
	if (args->time_to_die < 10 || args->time_to_eat < 10 || args->time_to_sleep < 10)
		return (print_error(ERR_TIME));
	// args->start_time = gettime_in_ms();
	if (/*pthread_mutex_init(&args->print_mtx, NULL)
		|| */pthread_mutex_init(&args->finish_mtx, NULL))
		return (print_error(ERR_MTX_INIT));
	return (true);
}

void	*philo_thread(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	// philo->last_eating_time = gettime_in_ms();
	if (philo->id % 2 == ODD)
		usleep_wrapper(philo->all->time_to_eat);
	while (true)
	{

		pthread_mutex_lock(&philo->all->finish_mtx);
	// printf("thread[%zu]\n", philo->id - 1);
		if (philo->all->finish == true)
		{

			pthread_mutex_unlock(&philo->all->finish_mtx);
			break ;
		}

		pthread_mutex_unlock(&philo->all->finish_mtx);

		pthread_mutex_lock(&philo->death_mtx);
		if (philo->all->meals > 0 && philo->meals == philo->all->meals)
		{
			pthread_mutex_unlock(&philo->death_mtx);
			break ;
		}
		philo->meals++;
		pthread_mutex_unlock(&philo->death_mtx);

		pthread_mutex_lock(philo->left_fork_mtx);
		print_status(philo, STATUS_FORK);

		pthread_mutex_lock(philo->right_fork_mtx);
		print_status(philo, STATUS_FORK);

		pthread_mutex_lock(&philo->death_mtx);
		philo->last_eating_time = gettime_in_ms();
		pthread_mutex_unlock(&philo->death_mtx);

		print_status(philo, STATUS_EAT);
		usleep_wrapper(philo->all->time_to_eat);

		pthread_mutex_unlock(philo->right_fork_mtx);
		pthread_mutex_unlock(philo->left_fork_mtx);

		print_status(philo, STATUS_SLEEP);
		usleep_wrapper(philo->all->time_to_sleep);
		print_status(philo, STATUS_THINK);
		if (philo->all->philo_count % 2 == ODD)
			usleep_wrapper(philo->all->time_to_eat);
	}
	return (NULL);
}

t_philo	*philos_init(t_all *all)
{
	size_t		i;
	t_philo		*philos;

	i = 0;
	all->finish = false;
	philos = malloc(sizeof(t_philo) * all->philo_count);
	all->forks = malloc(sizeof(pthread_mutex_t) * all->philo_count);
	if (!philos || !all->forks)
		return ((t_philo *)print_error(ERR_MALLOC));
	all->start_time = gettime_in_ms();
	while (i < all->philo_count)
	{
		if (pthread_mutex_init(&all->forks[i], NULL))
			return ((t_philo *)print_error(ERR_MTX_INIT));
		if (pthread_mutex_init(&philos[i].death_mtx, NULL))
			return ((t_philo *)print_error(ERR_MTX_INIT));
		philos[i].left_fork_mtx = &all->forks[i];
		philos[i].right_fork_mtx = &all->forks[(i + 1) % all->philo_count];
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].all = all;
		philos[i].dead = false;
		philos[i].last_eating_time = gettime_in_ms();
		i++;
	}
	// all->start_time = gettime_in_ms();
	return (philos);
}

bool	clean_philos(t_all *all, t_philo *philos)
{
	unsigned int	i;
	// unsigned int	dead;


	i = 0;
	// dead = 0;
	while (i < all->philo_count)
	{
		if (pthread_mutex_destroy(&all->forks[i]))
			return (print_error(ERR_MTX_DSTR));

		if (pthread_mutex_destroy(&philos[i].death_mtx))
		{
			return (print_error(ERR_MTX_DSTR));
		}
		// if (philos[i].dead)
		// 	dead = i;
		i++;
	}

	if (/*pthread_mutex_destroy(&all->print_mtx)
	|| */pthread_mutex_destroy(&all->finish_mtx))
	{
		return (print_error(ERR_MTX_DSTR));
	}

	free(philos);
	free(all->forks);
	return (true);
}

bool	threads_init(t_philo *philos)
{
	size_t	i;

// philos->all->start_time = gettime_in_ms();
	i = 0;
	while (i < philos->all->philo_count)
	{
		if (pthread_create(&philos[i].thread, NULL, &philo_thread, &philos[i]))
			return(print_error(ERR_THRD_CRT));

		i++;
	}
	return (true);
}

void	check_philos(t_philo *philos, t_all *all)
{
	size_t	i;
	bool	all_meals_eaten;


	while (true)
	{
		i = 0;
		all_meals_eaten = true;

		while (i < all->philo_count)
		{
			pthread_mutex_lock(&philos[i].death_mtx);
			if (gettime_in_ms() - philos[i].last_eating_time > all->time_to_die)
			{
				philos[i].dead = true;
				print_status(&philos[i], STATUS_DEATH);
				pthread_mutex_lock(&all->finish_mtx);
				all->finish = true;
				pthread_mutex_unlock(&all->finish_mtx);
				pthread_mutex_unlock(&philos[i].death_mtx);

				return ;
			}

			pthread_mutex_lock(&all->finish_mtx);
			if (philos[i].meals < all->meals)
			{
				all_meals_eaten = false;
			}
			pthread_mutex_unlock(&all->finish_mtx);
			pthread_mutex_unlock(&philos[i].death_mtx);
			i++;
		}

		pthread_mutex_lock(&all->finish_mtx);
		if (all->finish == true || (all_meals_eaten && all->meals >= 0))
		{
			pthread_mutex_unlock(&all->finish_mtx);
			return ;
		}
		pthread_mutex_unlock(&all->finish_mtx);
	}
}

int	main(int argc, char **argv)
{
	t_all			all;
	t_philo			*philos;

	if (!parser(argc, argv, &all))
	{
		if (!all.meals)
			return (0);
		return (1);
	}
	philos = philos_init(&all);
	if (!philos)
		return (1);

	threads_init(philos);

check_philos(philos, &all);

	if (all.philo_count == 1)
	{
		usleep(1000);
		pthread_mutex_unlock(&all.forks[0]);
	}
	for (size_t i = 0; i < all.philo_count; i++) {
		if (pthread_join(philos[i].thread, NULL)) {
			printf("thread join failed\n");
		}
		// printf ("thread[%zu] joined\n", i);
	}

	if (!clean_philos(&all, philos))
		return (1);

	return (0);
}
