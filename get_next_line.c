/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 14:49:57 by tvermeil          #+#    #+#             */
/*   Updated: 2015/12/12 19:12:19 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h> //

static t_pendant	*pendant;

/*
** fonction qui coupe une chaine en deux sur le \n
** et stock la deuxieme partie dans pendant
** retourne le premier bout de chaine
*/

/*
** free pendant et renvoie une chaine malloc !
** ne free pas la str car peut etre le buffer.
*/

static t_pendant	*get_pendant_fd(int fd)
{
	t_pendant	*parser;

	parser = pendant;
	while (parser != NULL && parser->fd != fd)
		parser = parser->next;
	//if (parser != NULL)
		//printf("got called to find pendant_fd %d and returned %p : %d, [%s]\n", fd, parser, parser->fd, parser->str);
	return (parser);
}

static int			add_pendant_fd(int fd, char *str)
{
	t_pendant	*new;
	t_pendant	*parser;

	//printf("-> adding new pendant on fd %d where str is [%s]\n", fd, str);
	new = (t_pendant*)malloc(sizeof(t_pendant));
	if (new == NULL)
		return (-1);
	//printf("malloc successful at %p\n", new);
	new->fd = fd;
	new->str = str;
	new->next = NULL;
	//printf("new->fd = %d, new->str = [%s]\n", new->fd, new->str);
	if (pendant == NULL)
	{
		//ft_putendl("pendant is at start");
		pendant = new;
		return (1);
	}
	parser = pendant;
	while (parser->next != NULL)
		parser = parser->next;
	parser->next = new;
	//ft_putendl("pendant is at end of list");
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

static char	*split_str(char *str, int fd)
{
	int			i;
	t_pendant	*previous_pendant; /* copie du pointeur pour le free */ // not needed anymore;

	//ft_putendl("----------- split_str ----------");
	//printf("got called on [%s] and fd %d\n", str, fd);
	previous_pendant = get_pendant_fd(fd);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			//ft_putendl("found backslash n");
			add_pendant_fd(fd, ft_strdup(&str[i + 1]));
			str[i] = '\0';
			break;
		}
		i++;
	}
	if (previous_pendant != NULL)
		remove_pendant_fd(fd);
	return (str);
}

int		get_next_line(int const fd, char **line)
{
	char		buffer[BUFF_SIZE + 1];
	char		*join;
	char		*cpy; /* ??? */;
	int			read_nbr;
	t_pendant	*pendant_fd;

	//ft_putendl("----------------- NEW CALL -----------------");
	if (line == NULL)
		return (-1);
	join = NULL;
	//ft_putendl("retrieving previous pendant");
	pendant_fd = get_pendant_fd(fd);
	if (pendant_fd != NULL)
	{
		//ft_putendl("pendant not empty, calling split on it");
		join = split_str(ft_strdup(pendant_fd->str), fd); // No free required;
	}
	//printf("About to read %d characters\n", BUFF_SIZE);
	read_nbr = 1; // cas ou rien n'est lu car pendant n'es pas vide // SUR ?;
	while ((pendant_fd = get_pendant_fd(fd)) == NULL && (read_nbr = read(fd, buffer, BUFF_SIZE)) != 0)
	{
		buffer[read_nbr] = '\0';
		cpy = join;
		//printf("Ok, just read %d chars : [%s]\n", read_nbr, buffer);
		if (read_nbr == -1)
		{
			//ft_putendl("got error while (reading");
			free(join);
			free(pendant_fd->str);
			free(pendant_fd);	/* si free pendant la fonction ne peut pas etre rappellee apres une erreur */;
			return (-1);	/* peut gagner une ligne en mettant ce block a la fin et faire un seul return */;
		}
		//ft_putendl("calling split on buffer");
		join = ft_strjoin(join, split_str(buffer, fd));
		//printf("join is now [%s]\n", join);
		free(cpy);
		//ft_putendl("about to read again");
	}
	//printf("at last join is [%s]\n", join);
	//printf("returning %d\n", (read_nbr > 0) ? 1 : 0);
	*line = join;
	return ((read_nbr > 0) ? 1 : 0);
}
