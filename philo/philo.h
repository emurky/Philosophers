#ifndef PHILO_H
# define PHILO_H

# include	<stdio.h>
# include	<stdlib.h>
# include	<limits.h>
# include	<sys/time.h>
# include	<pthread.h>

typedef struct s_philo
{
	pthread_mutex_t		mutex;
	int					sum;
}						t_philo;

char			*ft_itoa(int n);
int				ft_atoi(const char *str);

#endif
