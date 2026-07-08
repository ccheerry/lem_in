/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turns.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:35 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Maximum number of ants the first num_paths paths can deliver by turn t.
** A path of length len lets one ant in per turn, so it delivers t - len + 1
** ants by turn t (when t >= len).
*/
static long	capacity(t_path *paths, int num_paths, int t)
{
	long	sum;
	int		i;

	sum = 0;
	i = 0;
	while (i < num_paths && paths)
	{
		if (t >= paths->len)
			sum += (long)(t - paths->len + 1);
		paths = paths->next;
		i++;
	}
	return (sum);
}

/*
** Fewest turns to push num_ants ants through the num_paths shortest paths:
** the smallest t whose capacity covers every ant. Binary searched between the
** shortest path length and that length plus the ant count.
*/
int	calc_turns(t_path *paths, int num_paths, int num_ants)
{
	int	lo;
	int	hi;
	int	mid;

	lo = paths->len;
	hi = paths->len + num_ants;
	while (lo < hi)
	{
		mid = (lo + hi) / 2;
		if (capacity(paths, num_paths, mid) >= num_ants)
			hi = mid;
		else
			lo = mid + 1;
	}
	return (lo);
}

static int	pick_min(int *cost, int k)
{
	int	best;
	int	i;

	best = 0;
	i = 1;
	while (i < k)
	{
		if (cost[i] < cost[best])
			best = i;
		i++;
	}
	return (best);
}

/*
** Hands the ants to the chosen paths one by one: each ant joins the path that
** would let it arrive earliest (shortest len + ants already queued). The
** result is stored in every path's ants_assigned.
*/
void	assign_counts(t_lem_in *lem, t_path **arr, int k)
{
	int	*cost;
	int	a;
	int	best;

	cost = malloc(sizeof(int) * k);
	if (!cost)
		error_exit(lem);
	a = 0;
	while (a < k)
	{
		cost[a] = arr[a]->len;
		arr[a]->ants_assigned = 0;
		a++;
	}
	a = 0;
	while (a < lem->num_ants)
	{
		best = pick_min(cost, k);
		arr[best]->ants_assigned++;
		cost[best]++;
		a++;
	}
	free(cost);
}
