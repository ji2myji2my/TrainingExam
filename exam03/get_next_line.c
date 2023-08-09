/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 10:17:09 by laprieur          #+#    #+#             */
/*   Updated: 2023/04/25 10:13:57 by jmennock         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	nl_in_str(char *s)
{
	int	i;

	i = 0;
	while (s != NULL && s[i] != '\0')
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static char	*join(char *line, char *buf)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (line != NULL && line[i] != '\0')
		i++;
	str = malloc(sizeof(char) * (i + BUFFER_SIZE + 1));
	i = 0;
	while (line != NULL && line[i] != '\0')
	{
		str[i] = line[i];
		i++;
	}
	j = 0;
	while (buf[j] != '\0')
	{
		str[i] = buf[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(line);
	return (str);
}

static int	cut(char *line, char **buf)
{
	int	i;
	int	j;

	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	*buf = malloc(sizeof(char) * (i + 2));
	j = 0;
	while (j <= i)
	{
		(*buf)[j] = line[j];
		j++;
	}
	(*buf)[j] = '\0';
	if (nl_in_str(line) == 0 || line[j] == '\0')
		return (1);
	i = 0;
	while (line[j] != '\0')
	{
		line[i] = line[j];
		i++;
		j++;
	}
	line[i] = '\0';
	return (0);
}

char	*get_next_line(int fd)
{
	char		*buf;
	int			read_bytes;
	static char	*line = NULL;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	while (1)
	{
		read_bytes = read(fd, buf, BUFFER_SIZE);
		if (read_bytes <= 0)
			break ;
		buf[read_bytes] = '\0';
		line = join(line, buf);
		if (nl_in_str(line) == 1)
			break ;
	}
	free(buf);
	buf = NULL;
	if (line != NULL)
		read_bytes = cut(line, &buf);
	if (read_bytes == 1)
	{
		free(line);
		line = NULL;
	}
	return (buf);
}
