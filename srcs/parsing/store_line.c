/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:51 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int	store_line(t_input *input, char *line)
{
	char	**new_lines;
	int		new_cap;
	char	*dup;

	if (input->count >= input->capacity)
	{
		if (input->capacity == 0)
			new_cap = INIT_CAP;
		else
			new_cap = input->capacity * 2;
		new_lines = malloc(sizeof(char *) * new_cap);
		if (!new_lines)
			return (0);
		if (input->lines)
		{
			ft_memcpy(new_lines, input->lines, sizeof(char *) * input->count);
			free(input->lines);
		}
		input->lines = new_lines;
		input->capacity = new_cap;
	}
	dup = ft_strdup(line);
	if (!dup)
		return (0);
	return (input->lines[input->count++] = dup, 1);
}
