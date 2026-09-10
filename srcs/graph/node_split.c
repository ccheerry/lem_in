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

/* Room id becomes in (id*2) -> out (id*2+1), capacity 1: one ant per room. */
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
