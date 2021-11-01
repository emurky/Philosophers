#include "philo.h"

bool	print_error(char *err_str)
{
	write(2, "Error: ", 7);
	write(2, err_str, ft_strlen(err_str));
	return (false);
}

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
