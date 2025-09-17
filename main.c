/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:58:34 by ymazzett          #+#    #+#             */
/*   Updated: 2025/09/17 17:19:16 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompt_and_read(void)
{
	char	buffer[1024];
	int		bytes_read;

	write(1, "> ", 2);
	bytes_read = read(0, buffer, 1023);
	return (0);
}

int	main(void)
{
	int	error;

	error = 0;
	while (!error)
		error = prompt_and_read();
	if (error == 1)
		return (1);
	return (0);
}
