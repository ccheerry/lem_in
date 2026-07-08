/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Insertion sort of the path list by length, shortest first, so that taking
** the first k paths always means "the k shortest".
*/
static t_path	*sort_paths(t_path *head)
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

/*
** Tries every count of paths from 1 to the maximum flow and keeps the one
** giving the fewest turns. More paths is not always better: a long extra
** path can delay the last ant.
*/
static int	best_count(t_path *head, int count, int ants)
{
	int	k;
	int	best_k;
	int	best_t;
	int	t;

	best_k = 1;
	best_t = -1;
	k = 1;
	while (k <= count)
	{
		t = calc_turns(head, k, ants);
		if (best_t < 0 || t < best_t)
		{
			best_t = t;
			best_k = k;
		}
		k++;
	}
	return (best_k);
}

int	select_paths(t_lem_in *lem)
{
	t_path	**arr;
	int		count;
	int		best_k;
	int		i;

	lem->paths = sort_paths(extract_paths(lem));
	arr = build_array(lem->paths, &count);
	if (!arr || count == 0)
		error_exit(lem);
	best_k = best_count(lem->paths, count, lem->num_ants);
	assign_counts(lem, arr, best_k);
	i = best_k;
	while (i < count)
	{
		free(arr[i]->rooms);
		free(arr[i]);
		i++;
	}
	arr[best_k - 1]->next = NULL;
	lem->num_paths = best_k;
	return (free(arr), best_k);
}
