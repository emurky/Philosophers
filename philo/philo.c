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

void	usleep_wrapper(size_t ms)
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
	// printf("%lld\n", philo->all->start_time);
	// pthread_mutex_lock(&philo->death_mtx);
	if (!philo->dead)
	{
		pthread_mutex_unlock(&philo->all->print_mtx);
	}
	// pthread_mutex_unlock(&philo->death_mtx);
	// else
	// {
	// 	printf("tut\n");
	// }
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
		usleep_wrapper(philo->all->time_to_eat);
	// print_status(philo, STATUS_FORK);
	while (true)
	{
			// pthread_mutex_lock(&philo->death_mtx);
			// if (philo->all->finish == true)
			// 	break ;
			// pthread_mutex_unlock(&philo->death_mtx);
		pthread_mutex_lock(philo->left_fork_mtx);
		print_status(philo, STATUS_FORK);
		pthread_mutex_lock(philo->right_fork_mtx);
		print_status(philo, STATUS_FORK);
		pthread_mutex_lock(&philo->last_meal_mtx);
		philo->last_eating_time = gettime_in_ms();
		pthread_mutex_unlock(&philo->last_meal_mtx);
		print_status(philo, STATUS_EAT);
		usleep_wrapper(philo->all->time_to_eat);
		pthread_mutex_unlock(philo->right_fork_mtx);
		pthread_mutex_unlock(philo->left_fork_mtx);
		pthread_mutex_lock(&philo->death_mtx);
		
		if (philo->all->meals > 0 && philo->meals == philo->all->meals)
			break ;
		philo->meals++;

		pthread_mutex_unlock(&philo->death_mtx);
		print_status(philo, STATUS_SLEEP);
		usleep_wrapper(philo->all->time_to_sleep);
		print_status(philo, STATUS_THINK);
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
	while (i < all->philo_count)
	{
		if (pthread_mutex_init(&all->forks[i], NULL))
			return ((t_philo *)print_error(ERR_MTX_INIT));
		if (pthread_mutex_init(&philos[i].last_meal_mtx, NULL))
			return ((t_philo *)print_error(ERR_MTX_INIT));
		if (pthread_mutex_init(&philos[i].death_mtx, NULL))
			return ((t_philo *)print_error(ERR_MTX_INIT));
		philos[i].left_fork_mtx = &all->forks[i];
		philos[i].right_fork_mtx = &all->forks[(i + 1) % all->philo_count];
		philos[i].id = i + 1;
		philos[i].meals = 1;
		philos[i].all = all;
		philos[i].dead = false;
		philos[i].last_eating_time = all->start_time;
		i++;
	}
	return (philos);
}

bool	clean_philos(t_all *all, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	if (pthread_mutex_destroy(&all->print_mtx))
		return (print_error(ERR_MTX_DSTR));
	while (i < all->philo_count)
	{
		if (pthread_mutex_destroy(&all->forks[i]))
			return (print_error(ERR_MTX_DSTR));
		if (pthread_mutex_destroy(&philos[i].last_meal_mtx))
			return (print_error(ERR_MTX_DSTR));
		if (pthread_mutex_destroy(&philos[i].death_mtx))
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

void	check_philos(t_philo *philos, t_all *all)
{
	size_t	i;

	
	while (true)
	{
		i = 0;
		while (i < all->philo_count)
		{
			pthread_mutex_lock(&philos[i].last_meal_mtx);
			// printf("%d truth?\n", gettime_in_ms() - philos[i].last_eating_time > all->time_to_die);
			if (gettime_in_ms() - philos[i].last_eating_time > all->time_to_die)
			{
				pthread_mutex_lock(&philos[i].death_mtx);
				philos[i].dead = true;
				print_status(&philos[i], STATUS_DEATH);
				all->finish = true;
				pthread_mutex_unlock(&philos[i].death_mtx);
				
				return ;
			}
			pthread_mutex_unlock(&philos[i].last_meal_mtx);
			pthread_mutex_lock(&philos[i].death_mtx);
			if (philos[i].meals == all->meals)
			{
				all->finish = true;
				
				return ;
			}
			pthread_mutex_unlock(&philos[i].death_mtx);
			i++;
			
		}
		pthread_mutex_lock(&philos[i].death_mtx);
		if (all->finish == true && all->meals >= 0)
		{
			
			return ;
		}
		pthread_mutex_unlock(&philos[i].death_mtx);
		
	}
	// printf("tuuut\n");
}

int	main(int argc, char **argv)
{
	t_all			all;
	t_philo			*philos;

	if (!parser(argc, argv, &all))
		return (1);

	philos = philos_init(&all);
	if (!philos)
		return (1);

	threads_init(philos);

check_philos(philos, &all);
sleep(3);

	// for (size_t i = 0; i < all.philo_count; i++) {
	// 	pthread_join(philos[i].thread, NULL);
	// }

	// usleep_wrapper(2000);
	// pthread_mutex_unlock(&philos[check].death_mtx);
	// pthread_mutex_unlock(&all.print_mtx);
	// if (!clean_philos(&all, philos))
	// 	return (1);

	return (0);
}
