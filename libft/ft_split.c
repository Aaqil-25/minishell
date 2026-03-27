/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:28:03 by ymazzett          #+#    #+#             */
/*   Updated: 2025/01/15 23:33:13 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_findstrings(char const *s, char c, char **arrayofptr)
{
	const char	*pread;
	char		**pwrite;

	pread = s;
	pwrite = arrayofptr;
	if (pread[0] != c && pread[0])
	{
		*pwrite = (char *)pread++;
		pwrite++;
	}
	while (*pread)
	{
		if (pread[0] == c && pread[1] != c && pread[1])
		{
			*pwrite = (char *)&pread[1];
			pwrite++;
		}
		pread++;
	}
	*pwrite = NULL;
	return (arrayofptr);
}

static char	*ft_findastring(char **arrayofptr, int i, char c)
{
	const char	*pstart;
	const char	*pend;
	char		*output;
	int			n;
	int			j;

	pstart = arrayofptr[i];
	pend = NULL;
	n = 0;
	while (pstart[n] != c && pstart[n])
		n++;
	pend = &pstart[n];
	output = (char *)malloc((n + 1) * sizeof(char));
	if (!output)
		return (NULL);
	j = 0;
	while (pstart + j != pend && pstart[j])
	{
		output[j] = pstart[j];
		j++;
	}
	output[j] = 0;
	return (output);
}

static int	populate(char **arrayofptr, char **arrayofstr, char c, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		arrayofstr[i] = ft_findastring(arrayofptr, i, c);
		if (!arrayofstr[i])
		{
			while (i > 0)
			{
				free(arrayofstr[i]);
				i--;
			}
			return (0);
		}
		i++;
	}
	arrayofstr[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char		**arrayofptr;
	char		**arrayofstr;
	size_t		size;

	if (!s)
		return (NULL);
	size = ft_strlen(s);
	arrayofptr = (char **)malloc((size + 1) * sizeof(char *));
	if (arrayofptr == NULL)
		return (NULL);
	ft_findstrings(s, c, arrayofptr);
	size = 0;
	while (arrayofptr[size])
		size++;
	arrayofstr = (char **)malloc((size + 1) * sizeof(char *));
	if (arrayofstr == NULL)
		return (NULL);
	if (!populate(arrayofptr, arrayofstr, c, size))
		return (NULL);
	free(arrayofptr);
	return (arrayofstr);
}
