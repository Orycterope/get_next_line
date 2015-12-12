/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 14:49:57 by tvermeil          #+#    #+#             */
/*   Updated: 2015/12/12 20:29:41 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_pendant	*pendant;

static t_pendant	*get_pendant_fd(int fd)
{
	t_pendant	*parser;

	parser = pendant;
	while (parser != NULL && parser->fd != fd)
		parser = parser->next;
	return (parser);
}

static int			add_pendant_fd(int fd, char *str)
{
	t_pendant	*new;
	t_pendant	*parser;

	new = (t_pendant*)malloc(sizeof(t_pendant));
	if (new == NULL)
		return (-1);
	new->fd = fd;
	new->str = str;
	new->next = NULL;
	if (pendant == NULL)
	{
		pendant = new;
		return (1);
	}
	parser = pendant;
	while (parser->next != NULL)
		parser = parser->next;
	parser->next = new;
	return (1);
}

static void			remove_pendant_fd(int fd)
{
	t_pendant	*lst_parser;
	t_pendant	*previous;
	t_pendant	*next;

	if (pendant == NULL)
		return ;
	lst_parser = pendant;
	previous = NULL;
	while (lst_parser != NULL)
	{
		next = lst_parser->next;
		if (lst_parser->fd == fd)
		{
			free(lst_parser->str);
			free(lst_parser);
			if (previous != NULL)
				previous->next = next;
			else
				pendant = next;
			return ;
		}
		else
			previous = lst_parser;
		lst_parser = next;
	}
}

static char			*split_str(char *str, int fd)
{
	int			i;
	t_pendant	*previous_pendant;

	previous_pendant = get_pendant_fd(fd);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			add_pendant_fd(fd, ft_strdup(&str[i + 1]));
			str[i] = '\0';
			break ;
		}
		i++;
	}
	if (previous_pendant != NULL)
		remove_pendant_fd(fd);
	return (str);
}

int					get_next_line(int const fd, char **line)
{
	char		buffer[BUFF_SIZE + 1];
	char		*join;
	char		*cpy;
	int			read_nbr;
	t_pendant	*pendant_fd;

	if (line == NULL)
		return (-1);
	join = NULL;
	pendant_fd = get_pendant_fd(fd);
	if (pendant_fd != NULL)
		join = split_str(ft_strdup(pendant_fd->str), fd);
	read_nbr = 1;
	while ((pendant_fd = get_pendant_fd(fd)) == NULL
			&& (read_nbr = read(fd, buffer, BUFF_SIZE)) > 0)
	{
		buffer[read_nbr] = '\0';
		cpy = join;
		join = ft_strjoin(join, split_str(buffer, fd));
		free(cpy);
	}
	*line = join;
	return ((read_nbr > 0) ? 1 : read_nbr);
}
