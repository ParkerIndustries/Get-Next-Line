/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 14:04:46 by xxxxxxxx          #+#    #+#             */
/*   Updated: 2017/12/06 03:04:04 by xxxxxxxx         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
Cette fonction sert à déplacer les caractères lus en réserve depuis flood
vers line (ça vide petit à petit le buffer).

La fonction parcourt flood jusqu'au premier '\n' trouvé, puis copie la
partie gauche vers line et la partie droite vers le nouveau buffer sv, 
qui remplace flood (qui est détruit) et la fonction retourne 1.

	-	si aucun '\n' n'est trouvé dans flood, alors il est entièrement 
		copié vers line puis est détruit

	-  line est agrandi pour avoir toujours au moins BUF_SIZE caractères 
		d'espace libre
*/

int		container(char **flood, char **line, int *x)
{
	int		i;
	char	*sv;

	i = 0;
	while ((*flood)[i])
		if ((*flood)[i] == '\n')
		{
			sv = ft_memalloc((ft_strlen(*flood) - i) * sizeof(char));
			if (!sv)
				return (-1);
			sv = ft_strcpy(sv, &((*flood)[i - 1]));
			*line = ft_strncpy(*line, *flood, i);
			(*line)[i++] = '\0';
			free(*flood);
			*flood = sv;
			return (1);
		}
		else
			i++;
	*x += i;
	*line = ft_strncpy(*line, *flood, i);
	*line = (char *)ft_realloc_safe(*line, *x, *x + BUFF_SIZE + 1);
	free(*flood);
	*flood = NULL;
	return (0);
}

/*
Au moment de l'appel, line est un buffer pouvant contenir plusieurs lignes.

La fonction cherche le premier '\n'. S'il est trouvé, tout ce qui se trouve
derrière est copié vers un nouveau buffer de flood et line est tronquée par
un '\0' (mais garde sa taille de (x + BUFF_SIZE + 1)).

Tant qu'il reste du flood, splitline ne sera jamais appelé. Ce qui donne à
splitline la garantie de créer un nouveau buffer de flood sans avoir à se 
soucier d'ajouter des caractères dedans.
*/

int		splitline(char **flood, char **line, int *x)
{
	int i;

	(*line)[*x] = '\0';
	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '\n')
		{
			if ((*line)[i + 1])
			{
				*flood = (char *)ft_memalloc((*x - i) * sizeof(char));
				if (!*flood)
					return (-1);
				*flood = ft_strncpy(*flood, (*line + i + 1), *x - i);
				(*flood)[*x - i - 1] = '\0';
			}
			(*line)[i] = '\0';
			return (1);
		}
		i++;
	}
	return (0);
}

/*
On alloue le tableau de pointeurs flood s'il n'est pa déjà alloué,
et alloue le buffer de BUFF_SIZE + 1 byte qui va être lu depuis le fichier
*/

int		init(const int fd, char ***line, char ***flood)
{
	if (fd < 0 || fd > 5000 || !*line || FDS <= 0 || BUFF_SIZE <= 0)
		return (-1);
	if (!*flood)
	{
		if (!(*flood = (char **)ft_memalloc(FDS * sizeof(char *))))
			return (-1);
	}
	if (!(**line = (char *)ft_memalloc((BUFF_SIZE + 1) * sizeof(char))))
	{
		free(*flood);
		return (-1);
	}
	return (0);
}

/*
On initialise (ce qui donne un buffer line d'une taille minimale de 
(BUFF_SIZE + 1)).

///WARNING///: Ca ne réutilise jamais le line d'un précédent appel, risque
				de fuite de mémoire si l'appelant ne s'en rend pas compte

On vide le flood tant qu'on en a, puis on démarre la boucle while():
		-	D'abord on lit au plus BUFF_SIZE dans le fichier
		-	Si on a lu au moins un caractère et qu'il n'y a pas eu d'erreur,
			on augmente notre compteur x de la taille lue (étant donné que 
			ce n'est ps -1)
		-	Systématiquement, on appelle splitline pour couper au premier 
			'\n' rencontré
		-	Si on a trouvé un '\n' dans le buffer, c'est bon. La fonction
			retourne et line contient la chaîne lue. Si splitline a
			échoué (dû à un échec de malloc()), on retourne la valeur
			d'erreur

Si on n'a pas trouvé de '\n':
		-	SOIT: si on a lu BUFF_SIZE caractères, alors il faut continuer 
			à lire: on agrandit le buffer line (toujours à (x + BUFF_SIZE + 1))
			et on poursuit la boucle
		-	SOIT: Si on n'a pas lu BUFF_SIZE caractères, c'est qu'il n'y a
			plus rien à lire. Si read a échoué, on retourne une valeur
			d'erreur.
			Sinon, on tronque line à compteur x et on retourne 1 ou 0 selon 
			que la ligne soit complètement vide ou non
*/

int		get_next_line(const int fd, char **line)
{
	static char	**flood;
	int			res;
	int			r;
	int			x;

	x = 0;
	if (init(fd, &line, &flood))
		return (-1);
	if (flood[fd] && (res = container(&(flood[fd]), line, &x)))
		return (res);
	while ((r = read(fd, (*line + x), BUFF_SIZE)) > 0 && (x += r))
		if ((res = splitline(&(flood[fd]), line, &x)))
			return (res);
		else if (r == BUFF_SIZE)
		{
			*line = (char *)ft_realloc_safe(*line, x * sizeof(char),
					(x + BUFF_SIZE + 1) * sizeof(char));
			if (!*line)
				return (-1);
		}
	if (r < 0 || ((*line)[x] = '\0'))
		return (-1);
	return (**line ? 1 : 0);
}
