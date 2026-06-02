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
			ft_memcpy(bigger, graph->rooms,
				sizeof(t_room *) * graph->num_rooms);
			free(graph->rooms);
		}
		graph->rooms = bigger;
		graph->rooms_cap = cap;
	}
	graph->rooms[graph->num_rooms] = room;
	return (1);
}

static t_room	*new_room(char *line, int id)
{
	t_room	*room;
	char	**elems;

	elems = ft_split(line, ' ');
	if (!elems || !elems[0] || !elems[1] || !elems[2])
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
	if (!push_room(&lem->graph, room))
		return (free(room->name), free(room), 0);
	lem->graph.num_rooms++;
	if (type == START_ROOM)
		lem->graph.start_id = room->id;
	else if (type == END_ROOM)
		lem->graph.end_id = room->id;
	printf("[room] id=%d name=%s x=%d y=%d type=%d\n",
		room->id, room->name, room->x, room->y, type);
	return (1);
}
