/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tbojimi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmassera <cmassera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 13:43:19 by cmassera          #+#    #+#             */
/*   Updated: 2023/04/03 15:12:14 by jmennock         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_calloc(int size)
{
	int		i;
	char	*s;

	i = 0;
	s = malloc (size);
	while (i < size)
		s[i++] = 0;
	return (s);
}

char	*ft_strjoin(char *line, char *buf)
{
	size_t	i;
	char	*str;
	int		linelen;
	int		buflen;

	i = 0;
	linelen = ft_strlen(line);
	buflen = ft_strlen(buf);
	str = malloc(linelen + buflen +1);
	while (line[i])
	{
		str[i] = line[i];
		i++;
	}
	while (buf[i - linelen])
	{
		str[i] = buf[i - linelen];
		i++;
	}
	free(line);
	str[i] = '\0';
	return (str);
}

int	ft_endline(char *line, char *buf)
{
	int		i;
	int		j;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
		{
			j = 0;
			buf[j++] = line[++i];
			if (line[i] == '\0')
				return (1);
			line[i++] = '\0';
			while (line[i])
				buf[j++] = line[i++];
			buf[j] = '\0';
			return (1);
		}
		i++;
	}
	return (0);
}

char	*get_next_line(int fd)
{
	size_t			buflen;
	static char		*buf = NULL;
	char			*line;

	line = ft_calloc(1);
	if (!buf)
		buf = ft_calloc(BUFFER_SIZE + 1);
	while (1)
	{
		line = ft_strjoin(line, buf);
		if (ft_endline(line, buf) != 0)
			return (line);
		buflen = read(fd, buf, BUFFER_SIZE);
		if (buflen < 1)
		{
			free(buf);
			buf = NULL;
			if (line[0])
				return (line);
			free(line);
			return (NULL);
		}
		buf[buflen] = '\0';
	}
}
