/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:59:10 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static t_edge	*new_edge(int to, int cap, int is_rev)
{
	t_edge	*edge;

	edge = malloc(sizeof(t_edge));
	if (!edge)
		return (NULL);
	edge->to = to;
	edge->cap = cap;
	edge->is_rev = is_rev;
	edge->rev = NULL;
	edge->next = NULL;
	return (edge);
}

/*
** Adds a directed edge from->to with the given capacity plus its residual
** reverse edge (to->from, capacity 0). The two are linked through ->rev so
** max-flow can push and cancel flow.
*/
int	add_edge(t_graph *graph, int from, int to, int cap)
{
	t_edge	*fwd;
	t_edge	*back;

	fwd = new_edge(to, cap, 0);
	back = new_edge(from, 0, 1);
	if (!fwd || !back)
		return (free(fwd), free(back), 0);
	fwd->rev = back;
	back->rev = fwd;
	fwd->next = graph->adj[from];
	graph->adj[from] = fwd;
	back->next = graph->adj[to];
	graph->adj[to] = back;
	return (1);
}

void	graph_init(t_lem_in *lem)
{
	t_graph	*graph;
	int		i;
	int		from;
	int		to;

	graph = &lem->graph;
	graph->num_nodes = graph->num_rooms * 2;
	graph->adj = ft_calloc(graph->num_nodes, sizeof(t_edge *));
	if (!graph->adj)
		error_exit(lem);
	i = 0;
	while (i < graph->num_links)
	{
		from = graph->links[i].from;
		to = graph->links[i].to;
		if (!add_edge(graph, from * 2 + 1, to * 2, 1) || !add_edge(graph, to * 2 + 1, from * 2, 1))
			error_exit(lem);
		i++;
	}
}
