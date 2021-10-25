#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>

# define THREADS_LIMIT	420

# define ERR_ARGS		"Wrong number of arguments. Must be 4 or 5\n"
# define ERR_NUM		"One of arguments is not numerical. Arguments must be \
positive integers\n"
# define ERR_MALLOC		"Malloc error\n"
# define ERR_MTX_INIT		"Mutex initialisation\n"
# define ERR_MTX_DSTR		"Mutex destroying\n"

typedef struct s_philo
{
	pthread_t			thread;
	pthread_mutex_t		death_mtx;
	pthread_mutex_t		*left_fork_mtx;
	pthread_mutex_t		*right_fork_mtx;
	size_t				meals;
	size_t				id;

	int					sum;
}				t_philo;

typedef struct s_all
{
	unsigned int		philo_count;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	int					meals;
	size_t				start_time;
	pthread_mutex_t		print_mtx;

	t_philo				*philos;
	pthread_mutex_t		*forks;
}				t_all;

size_t			ft_strlen(char *str);
bool			print_error(char *err_str);
bool			str_isnumeric(char *str);
char			*ft_itoa(int n);
int				ft_atoi(const char *str);

#endif
