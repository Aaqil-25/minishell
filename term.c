/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdur-r <mabdur-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:45:12 by mabdur-r          #+#    #+#             */
/*   Updated: 2026/02/09 16:45:15 by mabdur-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

#ifdef ECHOCTL

static void	disable_echoctl(struct termios *t)
{
	t->c_lflag &= ~(ECHOCTL);
}
#else

static void	disable_echoctl(struct termios *t)
{
	(void)t;
}
#endif

#if defined(VQUIT) && defined(_POSIX_VDISABLE)

static void	disable_vquit(struct termios *t)
{
	t->c_cc[VQUIT] = _POSIX_VDISABLE;
}
#else

static void	disable_vquit(struct termios *t)
{
	(void)t;
}
#endif

void	term_apply(int prompt_mode)
{
	struct termios	t;

	if (tcgetattr(STDIN_FILENO, &t) != 0)
		return ;
	disable_echoctl(&t);
	if (prompt_mode)
		disable_vquit(&t);
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
