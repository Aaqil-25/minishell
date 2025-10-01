/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:11:01 by ymazzett          #+#    #+#             */
/*   Updated: 2025/06/06 21:08:37 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_filedata
{
	int		file_d;
	char	*buffer;
	size_t	read_pos;
	int		end_of_read;
	int		end_of_file;
}			t_filedata;

size_t	ft_strlen(const char *str);
char	*ft_strndup(char *str, size_t n);
char	*ft_strchr(char *str, int ch);
char	*double_free(char **ptr1, char **ptr2);
char	*string_concat(char *s1, char *s2);
char	*get_next_line(int fd);

#endif