/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:40 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** A forward edge (is_rev == 0) whose capacity dropped to 0 carries one unit of
** flow. Reverse edges keep is_rev == 1, so they are never mistaken for flow.
*/
static t_edge	*find_flow_edge(t_graph *g, int node)
{
	t_edge	*e;

	e = g->adj[node];
	while (e)
	{
		if (e->is_rev == 0 && e->cap == 0)
			return (e);
		e = e->next;
	}
	return (NULL);
}

/*
** Walks one flow route from out(start) to in(end), storing every split-node
** id and consuming each edge (cap back to 1) so the next call finds another
** route. Returns the node count, or 0 when no route leaves the source.
*/
static int	walk_flow(t_graph *g, int src, int sink, int *nodes)
{
	t_edge	*e;
	int		cur;
	int		n;

	cur = src;
	n = 0;
	nodes[n++] = cur;
	while (cur != sink)
	{
		e = find_flow_edge(g, cur);
		if (!e)
			return (0);
		e->cap = 1;
		cur = e->to;
		nodes[n++] = cur;
	}
	return (n);
}

static t_path	*build_path(int *nodes, int n)
{
	t_path	*path;
	int		i;
	int		count;

	path = ft_calloc(1, sizeof(t_path));
	if (!path)
		return (NULL);
	path->rooms = malloc(sizeof(int) * n);
	if (!path->rooms)
		return (free(path), NULL);
	count = 0;
	i = 0;
	while (i < n)
	{
		if (count == 0 || path->rooms[count - 1] != nodes[i] / 2)
			path->rooms[count++] = nodes[i] / 2;
		i++;
	}
	path->len = count - 1;
	return (path);
}

t_path	*extract_paths(t_lem_in *lem)
{
	t_path	*head;
	t_path	*path;
	int		*nodes;
	int		sink;
	int		n;

	sink = lem->graph.end_id * 2;
	nodes = malloc(sizeof(int) * lem->graph.num_nodes);
	if (!nodes)
		error_exit(lem);
	head = NULL;
	n = walk_flow(&lem->graph, lem->graph.start_id * 2 + 1, sink, nodes);
	while (n > 0)
	{
		path = build_path(nodes, n);
		if (!path)
			return (free(nodes), error_exit(lem), NULL);
		path->next = head;
		head = path;
		n = walk_flow(&lem->graph, lem->graph.start_id * 2 + 1, sink, nodes);
	}
	return (free(nodes), head);
}
