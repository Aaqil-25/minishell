/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:11:06 by ymazzett          #+#    #+#             */
/*   Updated: 2025/03/21 17:05:09 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_filedata	*file_init(int fd, t_filedata *file)
{
	if (file->end_of_file)
		return (NULL);
	if (file->file_d == 0 && !(file->buffer))
	{
		file->file_d = fd;
		free(file->buffer);
		file->buffer = ft_strndup("", 0);
		if (!file->buffer)
			return (NULL);
		file->read_pos = 0;
		file->end_of_read = 0;
		file->end_of_file = 0;
	}
	return (file);
}

char	*buffer_update(t_filedata *file)
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
			return (double_free(&file->buffer, &new_rd));
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

char	*get_line(t_filedata *file)
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
	static t_filedata	file_object;
	t_filedata			*file;
	char				*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	file = file_init(fd, &file_object);
	if (!file)
		return (NULL);
	if (!buffer_update(file))
		return (NULL);
	line = get_line(file);
	if (file->end_of_file)
		double_free(&file->buffer, NULL);
	return (line);
}
