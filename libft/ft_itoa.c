/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 00:36:13 by deck              #+#    #+#             */
/*   Updated: 2025/01/15 21:21:35 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_numrevlen(long *num, int *len, long *res)
{
	if (*num != 0)
	{
		*len += 1;
		*res *= 10;
		*res += *num % 10;
		*num = *num / 10;
		ft_numrevlen(num, len, res);
	}
}

static int	getsign(long *num)
{
	int		signal;

	signal = 1;
	if (*num < 0)
	{
		*num = -(*num);
		signal = -1;
	}
	return (signal);
}

static char	*fillstr(char *str, int len, long num, int signal)
{
	int		i;

	i = 0;
	if (signal < 0)
		str[i++] = '-';
	while (i < len)
	{
		str[i++] = (num % 10) + '0';
		num /= 10;
	}
	str[i] = 0;
	return (str);
}

char	*ft_itoa(int n)
{
	int			signal;
	long		num;
	int			len;
	long		reversed;
	char		*str;

	num = n;
	signal = getsign(&num);
	len = 0;
	reversed = 0;
	ft_numrevlen(&num, &len, &reversed);
	if (n == 0)
		len = 1;
	if (signal < 0)
		len++;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	fillstr(str, len, reversed, signal);
	return (str);
}
