/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:24:18 by emurky            #+#    #+#             */
/*   Updated: 2021/11/02 01:24:31 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>

# define THREADS_LIMIT	421

# define EVEN			0
# define ODD			1

# define STATUS_FORK	"has taken a fork"
# define STATUS_EAT		"is eating"
# define STATUS_SLEEP	"is sleeping"
# define STATUS_THINK	"is thinking"
# define STATUS_DEATH	"died"

# define ERR_PHILO		"At least one philosopher must exist.\n"
# define ERR_ARGS		"Wrong number of arguments. Must be 4 or 5.\n"
# define ERR_NUM		"Arguments must be numerical strings.\n"
# define ERR_INT_OVRFLW	"Integer overflow in one of arguments.\n"
# define ERR_MALLOC		"Malloc error.\n"
# define ERR_MTX_INIT	"Mutex initialisation.\n"
# define ERR_MTX_DSTR	"Mutex destroying.\n"
# define ERR_THRD_CRT	"Thread initialisation.\n"
# define ERR_THRD_JOIN	"Thread joining.\n"
# define ERR_TIME		"Time arguments must be at least 10 ms.\n"
# define ERR_THREADS	"There are too many philos. \
You can change THREADS_LIMIT in a header file.\n"

typedef struct s_all
{
	size_t				philo_count;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					meals;
	size_t				start_time;
	bool				finish;
	pthread_mutex_t		finish_mtx;
	pthread_mutex_t		*forks;
}				t_all;

typedef struct s_philo
{
	size_t				id;
	pthread_t			thread;
	pthread_mutex_t		death_mtx;
	pthread_mutex_t		*left_fork_mtx;
	pthread_mutex_t		*right_fork_mtx;
	size_t				last_eating_time;
	int					meals;
	bool				dead;
	t_all				*all;
}				t_philo;

/*				philo.c */
bool			parser(char **argv, t_all *args);
t_philo			*philos_init(t_all *all);
void			check_philos(t_philo *philos, t_all *all);
void			is_dead(t_philo *philo, t_all *all);

/*				threads.c */
bool			threads_init(t_philo *philos, t_all *all);
void			*philo_thread(void *arg);
bool			eating_time(t_philo *philo);
void			sleeping_and_maybe_thinking(t_philo *philo);
bool			threads_finish(t_philo *philos, t_all *all);

/*				utils.c */
bool			clean_philos(t_all *all, t_philo *philos);
size_t			gettime_in_ms(void);
void			usleep_wrapper(size_t ms);
void			print_status(t_philo *philo, char *status);
bool			print_error(char *err_str);

/*				libft.c */
bool			args_are_numeric(char **argv);
size_t			ft_strlen(char *str);
int				ft_atoi(const char *str);

#endif
