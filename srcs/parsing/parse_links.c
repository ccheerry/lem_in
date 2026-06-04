#include "lem_in.h"

static char	**split_link(char *line)
{
	char	**ends;

	ends = ft_split(line, '-');
	if (!ends || !ends[0] || !ends[1] || ends[2])
		return (free_split(ends), NULL);
	return (ends);
}

static int	grow_links(t_graph *graph)
{
	t_link	*bigger;
	int		cap;

	if (graph->links_cap == 0)
		cap = LINKS_INIT_CAP;
	else
		cap = graph->links_cap * 2;
	bigger = ft_calloc(cap, sizeof(t_link));
	if (!bigger)
		return (0);
	if (graph->links)
	{
		ft_memcpy(bigger, graph->links, sizeof(t_link) * graph->num_links);
		free(graph->links);
	}
	graph->links = bigger;
	graph->links_cap = cap;
	return (1);
}

static int	push_link(t_graph *graph, int a, int b)
{
	if (graph->num_links >= graph->links_cap && !grow_links(graph))
		return (0);
	graph->links[graph->num_links].from = a;
	graph->links[graph->num_links].to = b;
	graph->num_links++;
	return (1);
}

int	parse_link(t_lem_in *lem, char *line)
{
	char	**ends;
	t_room *a;
	t_room *b;

	ends = split_link(line);
	if (!ends)
		return (0);
	a = hash_lookup(lem->hash, ends[0]);
	b = hash_lookup(lem->hash, ends[1]);
	free_split(ends);
	if (!a || !b || a == b)
		return (0);
	return (push_link(&lem->graph, a->id, b->id));
}
