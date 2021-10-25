#include "philo.h"

bool	print_error(char *err_str)
{
	write(2, "Error: ", 7);
	write(2, err_str, ft_strlen(err_str));
	return (false);
}

bool	str_isnumeric(char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (!('0' <= *str && *str <= '9'))
			return (false);
		str++;
	}
	return (true);
}
