/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:10:44 by ymazzett          #+#    #+#             */
/*   Updated: 2025/03/21 18:27:04 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_filedata
{
	int					file_d;
	char				*buffer;
	size_t				read_pos;
	int					end_of_read;
	int					end_of_file;
	struct s_filedata	*next;
}						t_filedata;

size_t	ft_strlen(char *str);
char	*ft_strndup(char *str, size_t n);
char	*ft_strchr(char *str, int ch);
void	*double_free(void *p1, void *p2);
char	*string_concat(char *s1, char *s2);
char	*get_next_line(int fd);

#endif