#ifndef PHILO_H
# define PHILO_H

# include	<stdio.h>
# include	<stdlib.h>
# include	<pthread.h>

typedef struct s_philo
{
	pthread_mutex_t		mutex;
	int					sum;
}						t_philo;

#endif
