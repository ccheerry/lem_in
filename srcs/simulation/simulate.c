/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:44 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Gives each path a contiguous block of ant ids. Ant j (0-based) on a path is
** released on turn j+1 and reaches the room at index (turn - j) of that path.
*/
void	assign_ants(t_lem_in *lem)
{
	t_path	*p;
	int		base;

	base = 1;
	p = lem->paths;
	while (p)
	{
		p->first_ant = base;
		base += p->ants_assigned;
		p = p->next;
	}
}

static int	emit_token(t_buf *b, int ant_id, char *name, int *first)
{
	if (!*first && !buf_append(b, " ", 1))
		return (0);
	*first = 0;
	if (!buf_append(b, "L", 1) || !buf_putnbr(b, ant_id))
		return (0);
	if (!buf_append(b, "-", 1) || !buf_append(b, name, ft_strlen(name)))
		return (0);
	return (1);
}

static int	emit_turn(t_buf *b, t_lem_in *lem, int t)
{
	t_path	*p;
	char	*name;
	int		j;
	int		first;

	first = 1;
	p = lem->paths;
	while (p)
	{
		j = t - p->len;
		if (j < 0)
			j = 0;
		while (j <= t - 1 && j < p->ants_assigned)
		{
			name = lem->graph.rooms[p->rooms[t - j]]->name;
			if (!emit_token(b, p->first_ant + j, name, &first))
				return (0);
			j++;
		}
		p = p->next;
	}
	return (buf_append(b, "\n", 1));
}

static int	echo_input(t_buf *b, t_lem_in *lem)
{
	char	*line;
	int		i;

	i = 0;
	while (i < lem->input.count)
	{
		line = lem->input.lines[i];
		if (!buf_append(b, line, ft_strlen(line)))
			return (0);
		if (!buf_append(b, "\n", 1))
			return (0);
		i++;
	}
	return (buf_append(b, "\n", 1));
}

void	simulate(t_lem_in *lem)
{
	t_buf	b;
	t_path	*p;
	int		turns;
	int		t;

	ft_memset(&b, 0, sizeof(t_buf));
	if (!echo_input(&b, lem))
		return (free(b.data), error_exit(lem));
	turns = 0;
	p = lem->paths;
	while (p)
	{
		if (p->ants_assigned > 0 && p->ants_assigned + p->len - 1 > turns)
			turns = p->ants_assigned + p->len - 1;
		p = p->next;
	}
	t = 1;
	while (t <= turns)
	{
		if (!emit_turn(&b, lem, t))
			return (free(b.data), error_exit(lem));
		t++;
	}
	write(1, b.data, b.len);
	free(b.data);
}
