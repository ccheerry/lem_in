#include "lem_in.h"

int	parse_room(t_lem_in *lem, char *line, int type)
{
	t_room *room;
	char **elems;

	room = ft_memset(&room, 0, sizeof(t_room));
	elems = ft_split(line, ' ');
	room->id = lem->graph.num_rooms++;
	room->name = elems[0];
	room->x = ft_atoi(elems[1]);
	room->y = ft_atoi(elems[2]);
	free(elems);
	lem->graph.rooms[room->id] = room;
	if (type == START_ROOM)
		lem->graph.start_id = room->id;
	else if (type == END_ROOM)
		lem->graph.end_id = room->id;
	return (1);
}
