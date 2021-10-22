#include "philo.h"

static size_t	ft_static_nbrlen(long int n)
{
	size_t	len;

	len = 1;
	n /= 10;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*str;
	size_t			len;
	unsigned int	un;

	len = ft_static_nbrlen(n);
	if (n < 0)
	{
		un = -1 * n;
		len++;
	}
	else
		un = n;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len)
	{
		str[--len] = un % 10 + '0';
		un /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
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
