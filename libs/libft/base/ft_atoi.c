
#include "libft.h"

int	ft_atoi(const char *s)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	sign = 1;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -sign;
		i++;
	}
	res = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + (s[i] -48);
		i++;
	}
	return (sign * res);
}
