/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/09 12:10:04 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/* Insertion sort by length, shortest route first. */
t_path	*sort_paths(t_path *head)
{
	t_path	*sorted;
	t_path	*cur;
	t_path	*next;
	t_path	**slot;

	sorted = NULL;
	cur = head;
	while (cur)
	{
		next = cur->next;
		slot = &sorted;
		while (*slot && (*slot)->len <= cur->len)
			slot = &(*slot)->next;
		cur->next = *slot;
		*slot = cur;
		cur = next;
	}
	return (sorted);
}

static t_path	**build_array(t_path *head, int *count)
{
	t_path	**arr;
	t_path	*p;
	int		i;

	*count = 0;
	p = head;
	while (p)
	{
		(*count)++;
		p = p->next;
	}
	arr = malloc(sizeof(t_path *) * (*count));
	if (!arr)
		return (NULL);
	i = 0;
	p = head;
	while (p)
	{
		arr[i++] = p;
		p = p->next;
	}
	return (arr);
}

/* Orders the winning routes shortest first and shares the ants out. */
int	select_paths(t_lem_in *lem)
{
	t_path	**arr;
	int		count;

	lem->paths = sort_paths(lem->paths);
	arr = build_array(lem->paths, &count);
	if (!arr || count == 0)
		return (free(arr), error_exit(lem), 0);
	assign_counts(lem, arr, count);
	lem->num_paths = count;
	return (free(arr), count);
}
