/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rooms.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:55 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 21:30:13 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static int	push_room(t_graph *graph, t_room *room)
{
	t_room	**bigger;
	int		cap;

	if (graph->num_rooms >= graph->rooms_cap)
	{
		if (graph->rooms_cap == 0)
			cap = ROOMS_INIT_CAP;
		else
			cap = graph->rooms_cap * 2;
		bigger = ft_calloc(cap, sizeof(t_room *));
		if (!bigger)
			return (0);
		if (graph->rooms)
		{
			ft_memcpy(bigger, graph->rooms, sizeof(t_room *) * graph->num_rooms);
			free(graph->rooms);
		}
		graph->rooms = bigger;
		graph->rooms_cap = cap;
	}
	graph->rooms[graph->num_rooms] = room;
	return (1);
}

static int	valid_coord(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '-' || s[0] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	valid_fields(char **elems)
{
	if (!elems || !elems[0] || !elems[1] || !elems[2] || elems[3])
		return (0);
	if (elems[0][0] == 'L')
		return (0);
	if (!valid_coord(elems[1]) || !valid_coord(elems[2]))
		return (0);
	return (1);
}

static t_room	*new_room(char *line, int id)
{
	t_room	*room;
	char	**elems;

	elems = ft_split(line, ' ');
	if (!valid_fields(elems))
		return (free_split(elems), NULL);
	room = ft_calloc(1, sizeof(t_room));
	if (!room)
		return (free_split(elems), NULL);
	room->id = id;
	room->name = ft_strdup(elems[0]);
	room->x = ft_atoi(elems[1]);
	room->y = ft_atoi(elems[2]);
	free_split(elems);
	if (!room->name)
		return (free(room), NULL);
	return (room);
}

int	parse_room(t_lem_in *lem, char *line, int type)
{
	t_room	*room;

	room = new_room(line, lem->graph.num_rooms);
	if (!room)
		return (0);
	if (hash_lookup(lem->hash, room->name))
		return (free(room->name), free(room), 0);
	if (type == START_ROOM && lem->graph.start_id != -1)
		return (free(room->name), free(room), 0);
	if (type == END_ROOM && lem->graph.end_id != -1)
		return (free(room->name), free(room), 0);
	if (!push_room(&lem->graph, room))
		return (free(room->name), free(room), 0);
	lem->graph.num_rooms++;
	hash_insert(lem->hash, room->name, room);
	if (type == START_ROOM)
		lem->graph.start_id = room->id;
	else if (type == END_ROOM)
		lem->graph.end_id = room->id;
	return (1);
}
