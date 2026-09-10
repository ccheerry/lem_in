/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shortest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 12:10:04 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/10 14:56:41 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void	reset(t_graph *g, t_flow *f)
{
	int	i;

	i = 0;
	while (i < g->num_nodes)
	{
		f->dist[i] = INF;
		f->parent[i] = NULL;
		f->done[i] = 0;
		i++;
	}
	i = 0;
	while (i < f->nbuck)
		f->bhead[i++] = -1;
	f->ecnt = 0;
}

static void	push(t_flow *f, int node, int d)
{
	f->enode[f->ecnt] = node;
	f->enext[f->ecnt] = f->bhead[d];
	f->bhead[d] = f->ecnt;
	f->ecnt++;
}

/* Johnson potentials turn every residual cost non-negative (+1 fwd, -1 rev). */
static void	relax(t_graph *g, t_flow *f, int node)
{
	t_edge	*e;
	int		d;

	e = g->adj[node];
	while (e)
	{
		if (e->cap > 0)
		{
			d = f->dist[node] + 1 - 2 * e->is_rev
				+ f->pot[node] - f->pot[e->to];
			if (d < f->dist[e->to] && d < f->nbuck)
			{
				f->dist[e->to] = d;
				f->parent[e->to] = e;
				push(f, e->to, d);
			}
		}
		e = e->next;
	}
}

static void	scan(t_graph *g, t_flow *f)
{
	int	d;
	int	i;
	int	v;

	d = 0;
	while (d < f->nbuck)
	{
		while (f->bhead[d] != -1)
		{
			i = f->bhead[d];
			v = f->enode[i];
			f->bhead[d] = f->enext[i];
			if (!f->done[v] && f->dist[v] == d)
			{
				f->done[v] = 1;
				if (v == f->sink)
					return ;
				relax(g, f, v);
			}
		}
		d++;
	}
}

/* One cheapest augmenting path; potentials shift by min(dist, dist[sink]). */
int	shortest_path(t_graph *g, t_flow *f)
{
	int	i;

	reset(g, f);
	f->dist[f->src] = 0;
	push(f, f->src, 0);
	scan(g, f);
	if (f->dist[f->sink] == INF)
		return (0);
	i = 0;
	while (i < g->num_nodes)
	{
		if (f->dist[i] > f->dist[f->sink])
			f->pot[i] += f->dist[f->sink];
		else
			f->pot[i] += f->dist[i];
		i++;
	}
	return (1);
}
