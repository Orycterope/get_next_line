/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 14:49:57 by tvermeil          #+#    #+#             */
/*   Updated: 2015/12/11 15:24:24 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*pendant;

/*
** fonction qui coupe une chaine en deux sur le \n
** et stock la deuxieme partie dans pendant
** retourne le premier bout de chaine
*/

/*
** free pendant et renvoie une chaine malloc !
** ne free pas la str car peut etre le buffer.
*/

static char	*split_str(char *str)
{
	char	*pendant_cpy; /* copie du pointeur pour le free */
	int		i;

	pendant_cpy = pendant;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
			pendant = ft_strdup(&str[i + 1]);
			free(pendant_cpy);
			return (str);
		}
		i++;
	}
	free(pendant);
	pendant = NULL;
	return (str);
}

int		get_next_line(int const fd, char **line)
{
	char	buffer[BUFF_SIZE + 1];
	char	*join;
	char	*cpy; /* ??? */
	int		read_nbr;

	if (line == NULL)
		return (-1);
	join = NULL;
	if (pendant != NULL)
		join = split_str(ft_strdup(pendant)); // No free required
	read_nbr = 1; // cas ou rien n'est lu car pendant n'es pas vide
	while (pendant == NULL && (read_nbr = read(fd, buffer, BUFF_SIZE)) != 0)
	{
		buffer[read_nbr] = '\0';
		cpy = join;
		if (read_nbr == -1)
		{
			free(join);
			free(pendant);	/* si free pendant la fonction ne peut pas etre rappellee apres une erreur */
			return (-1);	/* peut gagner une ligne en mettant ce block a la fin et faire un seul return */
		}
		join = ft_strjoin(join, split_str(buffer));
		free(cpy);
	}
	*line = join;
	return ((read_nbr > 0) ? 1 : 0);
}
