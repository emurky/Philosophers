/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:24:18 by emurky            #+#    #+#             */
/*   Updated: 2022/03/07 12:39:04 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>
# include <signal.h>
# include <semaphore.h>

# define PROCESSES_LIMIT	421

# define STATUS_FORK	"has taken a fork"
# define STATUS_EAT		"is eating"
# define STATUS_SLEEP	"is sleeping"
# define STATUS_THINK	"is thinking"
# define STATUS_DEATH	"died"

# define MEALS_EATEN	0
# define PHILO_DIED		1

# define SEM_NAME_LEN	10

# define ERR_PHILO		"At least one philosopher must exist.\n"
# define ERR_ARGS		"Wrong number of arguments. Must be 4 or 5.\n"
# define ERR_NUM		"Arguments must be numerical strings.\n"
# define ERR_INT_OVRFLW	"Integer overflow in one of arguments.\n"
# define ERR_MALLOC		"Malloc error.\n"

# define ERR_TIME		"Time arguments must be at least 10 ms.\n"
# define ERR_SEM_VALUE	"Semaphore value overflow\n"
# define ERR_SEM_LEN	"There are too many philos. \
You can change SEM_NAME_LEN in a header file.\n"
# define ERR_PROCESSES	"There are too many philos. \
You can change PROCESSES_LIMIT in a header file.\n"

typedef struct s_all
{
	size_t		philo_count;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			meals;
	size_t		start_time;
	sem_t		*print_sem;
	sem_t		*forks;
}				t_all;

typedef struct s_philo
{
	size_t		id;
	pid_t		pid;
	sem_t		*death_sem;
	char		*death_sem_name[SEM_NAME_LEN];
	size_t		last_eating_time;
	int			meals;
	t_all		*all;
}				t_philo;

/*				philo.c */
bool			parser(char **argv, t_all *args);
t_philo			*philos_init(t_all *all);
void			clean_philos(t_all *all, t_philo *philos);

/*				processes.c */
bool			processes_init(t_philo *philos, t_all *all);
void			launch_processes_and_check(t_philo *philo);
void			*philo_thread(void *arg);
void			processes_finish(t_philo *philos, t_all *all);

/*				utils.c */
size_t			gettime_in_ms(void);
void			usleep_wrapper(size_t ms);
void			print_status(t_philo *philo, char *status);
bool			print_error(char *err_str, t_philo *philos);
void			create_semaphore_name(t_philo *philo);

/*				libft.c */
sem_t			*sem_open_wrapper(const char *name, size_t count);
bool			args_are_numeric(char **argv);
size_t			ft_strlen(char *str);
size_t			ft_nbrlen(long int n);
int				ft_atoi(const char *str);

#endif
