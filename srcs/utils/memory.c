/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:31 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	free_adj(t_graph *g)
{
	t_edge	*e;
	t_edge	*next;
	int		i;

	i = 0;
	while (g->adj && i < g->num_nodes)
	{
		e = g->adj[i];
		while (e)
		{
			next = e->next;
			free(e);
			e = next;
		}
		i++;
	}
	free(g->adj);
}

void	free_paths(t_path *p)
{
	t_path	*next;

	while (p)
	{
		next = p->next;
		free(p->rooms);
		free(p);
		p = next;
	}
}

void	free_all(t_lem_in *lem)
{
	t_graph	*g;
	int		i;

	g = &lem->graph;
	i = 0;
	while (i < lem->input.count)
	{
		free(lem->input.lines[i]);
		i++;
	}
	free(lem->input.lines);
	free_adj(g);
	i = 0;
	while (i < g->num_rooms)
	{
		free(g->rooms[i]->name);
		free(g->rooms[i]);
		i++;
	}
	free(g->rooms);
	free(g->links);
	hash_free(lem->hash);
	free_paths(lem->paths);
}
