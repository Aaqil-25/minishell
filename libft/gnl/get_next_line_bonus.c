/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:11:18 by ymazzett          #+#    #+#             */
/*   Updated: 2025/03/21 16:07:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*buffer_update(t_filedata *file)
{
	char	*new_rd;
	char	*new_buffer;
	ssize_t	rd_bytes;

	while (file->buffer && !ft_strchr(file->buffer + file->read_pos, '\n')
		&& !file->end_of_read)
	{
		new_rd = malloc((BUFFER_SIZE + 1) * sizeof(char));
		rd_bytes = read(file->file_d, new_rd, BUFFER_SIZE);
		if (rd_bytes == -1)
			return ((char *)double_free(&file->buffer, &new_rd));
		new_rd[rd_bytes] = 0;
		new_buffer = string_concat(file->buffer + file->read_pos, new_rd);
		file->read_pos = 0;
		double_free(&file->buffer, &new_rd);
		if (!new_buffer)
			return (NULL);
		file->buffer = new_buffer;
		if (rd_bytes == 0)
			file->end_of_read = 1;
	}
	return (file->buffer);
}

static t_filedata	*file_init(int fd)
{
	t_filedata	*file;

	file = (t_filedata *)malloc(sizeof(t_filedata));
	if (!file)
		return (NULL);
	file->file_d = fd;
	file->read_pos = 0;
	file->end_of_read = 0;
	file->end_of_file = 0;
	file->next = NULL;
	file->buffer = ft_strndup("", 0);
	if (!file->buffer)
		return ((t_filedata *)double_free(&file, NULL));
	if (!buffer_update(file))
		return ((t_filedata *)double_free(&file->buffer, &file));
	return (file);
}

static t_filedata	*get_filedata(int fd, t_filedata **file_list)
{
	if (*file_list)
	{
		if (fd == (*file_list)->file_d)
			return (*file_list);
		else if ((*file_list)->next)
		{
			return (get_filedata(fd, &((*file_list)->next)));
		}
		else
		{
			(*file_list)->next = file_init(fd);
			return ((*file_list)->next);
		}
	}
	else
	{
		*file_list = file_init(fd);
		return (*file_list);
	}
}

static char	*get_line(t_filedata *file)
{
	size_t	start;
	char	*end;
	char	*line;

	start = file->read_pos;
	end = ft_strchr(file->buffer + file->read_pos, '\n');
	if (!end && !file->end_of_read)
		return (NULL);
	if (end)
		file->read_pos = end - file->buffer + 1;
	else
	{
		file->read_pos = ft_strlen(file->buffer);
		file->end_of_file = 1;
	}
	line = NULL;
	if ((file->buffer)[start] != 0)
		line = ft_strndup(file->buffer + start, file->read_pos - start);
	return (line);
}

char	*get_next_line(int fd)
{
	static t_filedata	*file_list;
	t_filedata			*file;
	char				*line;
	t_filedata			**tmp_a;
	t_filedata			*tmp_b;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	file = get_filedata(fd, &file_list);
	if (!file)
		return (NULL);
	if (!buffer_update(file))
		return (NULL);
	line = get_line(file);
	if (file->end_of_file)
	{
		tmp_b = file->next;
		double_free(&file->buffer, NULL);
		tmp_a = &file_list;
		while (*tmp_a != file)
			*tmp_a = (*tmp_a)->next;
		free(*tmp_a);
		*tmp_a = tmp_b;
	}
	return (line);
}
