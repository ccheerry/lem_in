/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flow.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 12:10:04 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/10 14:56:30 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void	init_flow(t_graph *g, t_flow *f)
{
	int	i;

	f->nbuck = g->num_nodes * 2 + 2;
	f->src = g->start_id * 2 + 1;
	f->sink = g->end_id * 2;
	f->ecnt = 0;
	i = 0;
	while (i < g->num_nodes)
		f->pot[i++] = 0;
}

/* Scratch arrays for the search: node tables, bucket heads and entry pool. */
int	alloc_flow(t_graph *g, t_flow *f)
{
	ft_memset(f, 0, sizeof(t_flow));
	f->parent = malloc(sizeof(t_edge *) * g->num_nodes);
	f->dist = malloc(sizeof(int) * g->num_nodes);
	f->pot = malloc(sizeof(int) * g->num_nodes);
	f->done = malloc(g->num_nodes);
	f->bhead = malloc(sizeof(int) * (g->num_nodes * 2 + 2));
	f->enode = malloc(sizeof(int) * (g->num_edges + 1));
	f->enext = malloc(sizeof(int) * (g->num_edges + 1));
	if (!f->parent || !f->dist || !f->pot || !f->done || !f->bhead
		|| !f->enode || !f->enext)
		return (free_flow(f), 0);
	init_flow(g, f);
	return (1);
}

void	free_flow(t_flow *f)
{
	free(f->parent);
	free(f->dist);
	free(f->pot);
	free(f->done);
	free(f->bhead);
	free(f->enode);
	free(f->enext);
}
