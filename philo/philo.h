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

# define EVEN			0
# define ODD			1

# define STATUS_FORK	"has taken a fork"
# define STATUS_EAT		"is eating"
# define STATUS_SLEEP	"is sleeping"
# define STATUS_THINK	"is thinking"
# define STATUS_DEATH	"died"

# define ERR_ARGS		"Wrong number of arguments. Must be 4 or 5\n"
# define ERR_NUM		"One of arguments is not numerical. Arguments must be \
positive integers\n"
# define ERR_MALLOC		"Malloc error\n"
# define ERR_MTX_INIT	"Mutex initialisation\n"
# define ERR_MTX_DSTR	"Mutex destroying\n"
# define ERR_THRD_CRT	"Thread creating\n"
# define ERR_THRD_DTC	"Thread detaching\n"

typedef struct s_all
{
	unsigned int		philo_count;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	int					meals;
	size_t				start_time;
	pthread_mutex_t		print_mtx;

	// t_philo				*philos;
	pthread_mutex_t		*forks;

	int					sum;
}				t_all;

typedef struct s_philo
{
	pthread_t			thread;
	pthread_mutex_t		death_mtx;
	pthread_mutex_t		*left_fork_mtx;
	pthread_mutex_t		*right_fork_mtx;
	size_t				meals;
	size_t				id;

	bool				dead;
	t_all				*all;

	int					sum;
}				t_philo;

size_t			ft_strlen(char *str);
bool			print_error(char *err_str);
bool			str_isnumeric(char *str);
char			*ft_itoa(int n);
int				ft_atoi(const char *str);

#endif
