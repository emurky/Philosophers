/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 12:38:52 by emurky            #+#    #+#             */
/*   Updated: 2022/03/07 12:38:56 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_all		all;
	t_philo		*philos;

	philos = NULL;
	memset(&all, 0, sizeof(all));
	if (argc != 5 && argc != 6)
		return (!print_error(ERR_ARGS, philos));
	if (!parser(argv, &all))
	{
		if (!all.meals)
			return (0);
		return (1);
	}
	philos = philos_init(&all);
	if (!philos)
		return (1);
	if (!processes_init(philos, &all))
		return (1);
	// check_philos(philos, &all);
	processes_finish(philos, &all);
	clean_philos(&all, philos);
	return (0);
}

bool	parser(char **argv, t_all *args)
{
	args->meals = -1;
	if (!args_are_numeric(argv))
		return (print_error(ERR_NUM, NULL));
	args->philo_count = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		args->meals = ft_atoi(argv[5]);
	if (args->time_to_die < 0 || args->time_to_eat < 0
		|| args->time_to_sleep < 0 || (args->meals < 0 && argv[5]))
		return (print_error(ERR_INT_OVRFLW, NULL));
	if (!args->meals)
		return (false);
	if (!args->philo_count)
		return (print_error(ERR_PHILO, NULL));
	if (args->philo_count > SEM_VALUE_MAX)
		return (print_error(ERR_SEM_VALUE, NULL));
	if (args->philo_count > PROCESSES_LIMIT)
		return (print_error(ERR_PROCESSES, NULL));
	if (ft_nbrlen(PROCESSES_LIMIT) > SEM_NAME_LEN - 7)
		return (print_error(ERR_SEM_LEN, NULL));
	if (args->time_to_die < 10 || args->time_to_eat < 10
		|| args->time_to_sleep < 10)
		return (print_error(ERR_TIME, NULL));
	return (true);
}

sem_t	*sem_open_wrapper(const char *name, size_t count)
{
	sem_unlink(name);
	return (sem_open(name, O_CREAT, 0644, count));
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

t_philo	*philos_init(t_all *all)
{
	size_t		i;
	t_philo		*philos;

	i = 0;
	philos = NULL;
	philos = malloc(sizeof(t_philo) * all->philo_count);
	all->forks = sem_open_wrapper("forks", all->philo_count);
	// all->death_sem = sem_open_wrapper("death_sem", 1);
	all->print_sem = sem_open_wrapper("print_sem", 1);
	if (!philos)
		return ((t_philo *)print_error(ERR_MALLOC, philos));
	all->start_time = gettime_in_ms();
	while (i < all->philo_count)
	{
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].all = all;
		create_semaphore_name(&philos[i]);
		philos[i].death_sem = sem_open_wrapper((char *)philos[i].death_sem_name, 1);
		// philos[i].dead = false;
		// philos[i].last_eating_time = gettime_in_ms();
		i++;
	}
	return (philos);
}

// void	is_dead(t_philo *philo, t_all *all)
// {
// 	// philo->dead = true;
// 	print_status(philo, STATUS_DEATH);
// 	// sem_wait(all->finish_sem);
// 	// all->finish = true;
// 	// sem_post(all->finish_sem);
// 	sem_post(all->death_sem);
// 	exit(1);
// }
