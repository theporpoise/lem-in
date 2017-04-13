/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgould <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/05 18:30:33 by mgould            #+#    #+#             */
/*   Updated: 2017/04/13 07:46:34 by mgould           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <lem-in.h>


void	debug_game(t_game *game)
{
	if (!game)
		return ;
	printf("\nYOU ARE IN DEBUG MODE\n");
	if (game->nbr)
		printf("ant number:%d\n", game->nbr);
	printf("Rooms\nName\t\t\tID\tX\tY\t\n");
	if (!(game->rmlst))
		return ;
	while (game->rmlst)
	{
		printf("%-24s%d\t%d\t%d\n", (game->rmlst)->nm, (game->rmlst)->id, \
			   	(game->rmlst)->x, (game->rmlst)->y);
		game->rmlst = (game->rmlst)->nx;
	}
	printf("START\n%-24s%d\t%d\t%d\n", (game->start)->nm, (game->start)->id, \
		   	(game->start)->x, (game->start)->y);
	printf("END\n%-24s%d\t%d\t%d\n", (game->end)->nm, (game->end)->id, \
		   	(game->end)->x, (game->end)->y);
}

int	ft_isnbr(char *ln)
{
	if (*ln == '+' || *ln == '-')
		ln++;
	while (*ln)
	{
		if (ft_isdigit(*ln))
			ln++;
		else
			return (0);
	}
	return (1);
}

int	valantcount(char *ln, int *command, t_game *game)
{
	if (!ft_strcmp("##end", ln) ||!ft_strcmp("##start", ln))
		return (0);
	if (*ln == '#')
		return (2);
	else if (ft_isnbr(ln))
	{
		*command = 1;
		if (ft_atoi(ln) < 1)
			return (0);
		game->nbr = ft_atoi(ln);
		return (1);
	}
	return (0);
}

int ft_getarraylen(char **array)
{
	int i;

	i = 0;
	while (array[i])
		i++;
	return i;
}


int	duprmcoords(t_room *tmp, t_room *game)

{
	t_room *xy;

	xy = game;
	while (xy)
	{
		if (xy->x == tmp->x && xy->y == tmp->y)
			return (1);
		xy = xy->nx;
	}
	xy = game;
	while (xy)
	{
		if (!ft_strcmp(xy->nm, tmp->nm))
			return (1);
		xy = xy->nx;
	}
	return (0);
}

int isroom(char *ln, t_game *game)
{
	char	**words;
	t_room	*tmp;

	if (*ln == '#')
		return (2);
	tmp = NULL;
	words = ft_strsplit(ln, ' ');
	if (*ln == 'L' || (ft_getarraylen(words) != 3) || ft_strchr(words[0], '-') \
			|| !ft_isnbr(words[1]) || !ft_isnbr(words[2]))
		return (0);
	tmp = makeroom(words[0], atoi(words[1]), atoi(words[2]));
	free(words[1]);
	free(words[2]);
	free(words);
	if (duprmcoords(tmp, game->rmlst))
		return (0);
	tmp->nx = game->rmlst;
	game->rmlst = tmp;
	return (1);
}



int valstartorend(int sore, t_game *game)
{
	char *check;
	int i;

	get_next_line(0, &check);
	if (!(ft_strcmp("##end", check) || !ft_strcmp("##start", check)))
		return (0);
	if ((i = isroom(check, game)) == 0)
		return (0);
	if (i == 2)
	{
		i = valstartorend(sore, game);
		free(check);
		return (i);
	}
	else if (i == 1 && sore == 1)
		game->start = game->rmlst;
	else if (i == 1 && sore == 0)
		game->end = game->rmlst;
	printf("%s\n", check);
	free(check);
	return (2);
}



int valroom(char *ln, int *command, t_game *game)
{
	static int start;
	static int end;

	if ((!ft_strcmp("##end", ln) && (end > 0)) ||
		(((!ft_strcmp("##start", ln)) && (start > 0))))
		return (0);
	else if (!(ft_strcmp("##start", ln)) && start == 0)
	{
		ft_printf("##start\n");
		start++;
		return (valstartorend(1, game));
	}
	else if (!(ft_strcmp("##end", ln)) && end == 0)
	{
		ft_printf("##end\n");
		end++;
		return (valstartorend(0, game));
	}
	else if (*ln == '#')
		return (2);
	else if (isroom(ln, game))
		return (1);
	*command = 2;
	return (3);
}


//TO DO
//finish psuedo code of links validation
//create a room edges integer map
//create init struct function for links
//create a roomexist function
//create a duplicate link function (can use room edges integer map)
//validate links according to rules
//add links to game struct
//update debug function to print out links in game struct
//make a coordinate map function to help visualize everything!
//make a cleanup function.
//

/*
 * Rooms can be separated by multiple spaces
 * Room name can NOT start with an L
 * Room names can NOT contain a '-' b/c undefined behavior for links
 * I ignore leading or trailing white space for rooms
 * Must provide 2 coordinates which are numbers.
 * I allow negative (-) and (+) numbers for coordinates.
 * I do NOT allow duplicate coordinates for 2 diff rooms b/c laws of physics!
 * I do NOT allow duplicate room names, even with diff coords,
 * 		b/c i wouldn't know where to send the ants
 * I IGNORE duplicate links or links to self
*/

/*
int roomexist(char *ln, t_game *game)
{

}
*/

//create edges map

/*
int islink(char *ln, t_game *game)
{
	char	**words;
	t_lst	*tmp;

	//check if it's a comment
	if (*ln == '#')
		return (2);
	tmp = NULL;
	words = ft_strsplit(ln, '-');
	//check that rooms exists and there are two of them
	if (*ln == 'L' || (ft_getarraylen(words) != 2) \
			|| !roomexist(words[0]) || !roomexist(words[1]))
		return (0);
	//check if it's a link to itself, don't create this, silently ignore
	else if (words[0] == words[1])
		;
	//check if I already have that link (in the integer edges map)
	tmp = makelst(words[0], atoi(words[1]), atoi(words[2]));

	tmp->nx = game->rmlst;
	if (!(game->rmlst))
		game->rmlst = tmp;
	else
		game->rmlst = tmp;
	free(words[1]);
	free(words[2]);
	free(words);
	return (1);

}
*/

int vallink(char *ln, t_game *game)
{
	char	**words;

	if (!(game->end) || !(game->start) || !(game->rmlst))
		return (0);
	words = ft_strsplit(ln, '-');
	if (!ft_strcmp("##end", ln) ||!ft_strcmp("##start", ln))
		return (0);
	if (*ln == '#')
		return (2);
	if (ft_getarraylen(words) != 2 || words[0][0] == 'L' || words[1][0] == 'L')
		return (0);

	//if (!roomexists(char *name))not a name of a room, then return 0
	return (1);
}

int	valinput(char *ln, t_game *game)
{
	static int command;
	int i;

	i = 0;
	if (command == 0)
	{
		return (valantcount(ln, &command, game));
	}
	else if (command == 1 && (i = valroom(ln, &command, game)) != 3)
	{
		return (i);
	}
	else if (command == 2)
	{
		printf("you are in link validation.");
		return (vallink(ln, game));
	}
	printf("did not get processed by valinput\n");
	return (0);
}

int main(void)
{
	char *line;
	int i;
	t_game	*game;

	game = makegame();

	i = 0;
	while (get_next_line(0, &line) > 0)
	{
		if (!(i = valinput(line, game)))
		{
			printf("Error\n");
			free(line);
			break ;
		}
		else if (i == 1)
			printf("%s\n", line);
		free(line);
	}

	debug_game(game);

	//create function that cleans up the game
	free(game);
	//game is potentially composed of multiple structs :-).
	return (0);
}




















