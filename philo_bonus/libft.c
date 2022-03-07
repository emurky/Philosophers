/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:24:34 by emurky            #+#    #+#             */
/*   Updated: 2021/11/02 01:24:38 by emurky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
			return (-1);
		number = number * 10 + *str++ - '0';
	}
	return ((int)(number * minus));
}
