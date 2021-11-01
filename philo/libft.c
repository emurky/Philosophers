#include "philo.h"

bool	args_are_numeric(char **argv)
{
	int		i;
	int		j;

	if (!argv)
		return (false);
	j = 1;
	while (argv[j])
	{
		i = 0;
		while (argv[j][i])
		{
			if (!('0' <= argv[j][i] && argv[j][i] <= '9'))
				return (false);
			i++;
		}
		j++;
	}
	return (true);
}

size_t	ft_strlen(char *str)
{
	char	*head;

	head = str;
	if (!str)
		return (0);
	while (*str)
		str++;
	return (str - head);
}

static int	int_overflow(int minus)
{
	if (minus == 1)
		return (INT_MAX);
	else
		return (INT_MIN);
}

int	ft_atoi(const char *str)
{
	int		minus;
	size_t	number;

	number = 0;
	minus = 1;
	while (*str == ' ' || *str == '\n' || *str == '\t'
		|| *str == '\v' || *str == '\f' || *str == '\r')
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			minus = -1;
		str++;
	}
	while (*str && ('0' <= *str && *str <= '9'))
	{
		if (number > INT_MAX / 10
			|| (number == INT_MAX / 10 && *str - '0' > INT_MAX % 10))
			return (int_overflow(minus));
		number = number * 10 + *str++ - '0';
	}
	return ((int)(number * minus));
}
