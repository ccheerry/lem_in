/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:59:02 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Splits every room into in (id*2) and out (id*2+1) joined by an internal
** edge of capacity 1, so at most one ant path can cross any single room.
** start/end carry no useful flow on this edge: the flow source is out(start)
** and the sink is in(end), so the limit never constrains them.
*/
void	node_split(t_lem_in *lem)
{
	t_graph	*graph;
	int		i;

	graph = &lem->graph;
	i = 0;
	while (i < graph->num_rooms)
	{
		if (!add_edge(graph, i * 2, i * 2 + 1, 1))
			error_exit(lem);
		i++;
	}
}
