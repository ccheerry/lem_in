/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:42 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 21:01:20 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static int	bfs(t_graph *g, int src, int sink, t_bfs *b)
{
	int		head;
	int		tail;
	t_edge	*e;

	ft_memset(b->visited, 0, g->num_nodes);
	head = 0;
	tail = 0;
	b->visited[src] = 1;
	b->queue[tail++] = src;
	while (head < tail)
	{
		e = g->adj[b->queue[head++]];
		while (e)
		{
			if (e->cap > 0 && !b->visited[e->to])
			{
				b->visited[e->to] = 1;
				b->parent[e->to] = e;
				b->queue[tail++] = e->to;
			}
			e = e->next;
		}
	}
	return (b->visited[sink]);
}

static void	augment(t_bfs *b, int src, int sink)
{
	int		node;
	t_edge	*e;

	node = sink;
	while (node != src)
	{
		e = b->parent[node];
		e->cap -= 1;
		e->rev->cap += 1;
		node = e->rev->to;
	}
}

/*
** Edmonds-Karp on the split graph from out(start) to in(end). Each shortest
** augmenting path adds one node-disjoint route; the returned flow is the
** number of such routes (0 means start and end are not connected).
*/
int	algorithm(t_lem_in *lem)
{
	t_bfs	b;
	int		src;
	int		sink;
	int		flow;

	src = lem->graph.start_id * 2 + 1;
	sink = lem->graph.end_id * 2;
	b.parent = malloc(sizeof(t_edge *) * lem->graph.num_nodes);
	b.queue = malloc(sizeof(int) * lem->graph.num_nodes);
	b.visited = malloc(lem->graph.num_nodes);
	if (!b.parent || !b.queue || !b.visited)
		return (free(b.parent), free(b.queue), free(b.visited), 0);
	flow = 0;
	while (bfs(&lem->graph, src, sink, &b))
	{
		augment(&b, src, sink);
		flow++;
	}
	return (free(b.parent), free(b.queue), free(b.visited), flow);
}
