/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 00:34:19 by msennane          #+#    #+#             */
/*   Updated: 2023/12/01 01:38:59 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

char	*extract_clean_line(char *buffer)
{
	int	i;

	i = 0;
	if (!buffer || !*buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	return (ft_substr(buffer, 0, i));
}

char	*extract_rest(char *s)
{
	char	*rest;
	int		i;
	int		len;

	i = 0;
	if (!s[i])
	{
		free(s);
		return (NULL);
	}
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		i++;
	len = ft_strlen(s + i) + 1;
	rest = malloc(sizeof(char) * len);
	if (!rest)
		return (NULL);
	ft_strlcpy(rest, s + i, len);
	free(s);
	return (rest);
}

char	*initialize_buffer(void)
{
	char	*buffer;

	if (BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	return (buffer);
}

char	*read_and_append_line(char *buffer, char *line, int fd,
		ssize_t *bytes_read)
{
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(buffer);
		return (NULL);
	}
	*bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (*bytes_read < 0)
	{
		free(buffer);
		free(line);
		return (NULL);
	}
	line[*bytes_read] = '\0';
	buffer = ft_strjoin(buffer, line);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	ssize_t		bytes_read;
	char		*line;

	line = initialize_buffer();
	if (!line)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(line);
		return (NULL);
	}
	bytes_read = 1;
	while (!ft_strchr(buffer, '\n') && bytes_read > 0)
	{
		buffer = read_and_append_line(buffer, line, fd, &bytes_read);
		if (!buffer)
			return (NULL);
	}
	free(line);
	line = extract_clean_line(buffer);
	buffer = extract_rest(buffer);
	return (line);
}
