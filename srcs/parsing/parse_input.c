/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:59:00 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static char	*next_line(char *data, size_t len, size_t *pos)
{
	size_t	start;

	if (*pos >= len)
		return (NULL);
	start = *pos;
	while (*pos < len && data[*pos] != '\n')
		(*pos)++;
	data[*pos] = '\0';
	if (*pos < len)
		(*pos)++;
	return (data + start);
}

static int	parse_ants(t_lem_in *lem, char *line)
{
	int	i;
	int	n;

	i = 0;
	if (!line || !line[0])
		return (0);
	while (line[i])
	{
		if (line[i] < '0' || line[i] > '9')
			return (0);
		i++;
	}
	n = ft_atoi(line);
	if (n <= 0)
		return (0);
	lem->num_ants = n;
	return (1);
}

static int	dispatch_line(t_lem_in *lem, char *line, int *type)
{
	if (ft_strcmp(line, "##start") == 0)
	{
		*type = START_ROOM;
		return (store_line(&lem->input, line));
	}
	if (ft_strcmp(line, "##end") == 0)
	{
		*type = END_ROOM;
		return (store_line(&lem->input, line));
	}
	if (line[0] == '#')
		return (store_line(&lem->input, line));
	if (ft_strchr(line, '-') && !ft_strchr(line, ' '))
		return (store_line(&lem->input, line) && parse_link(lem, line));
	if (!store_line(&lem->input, line) || !parse_room(lem, line, *type))
		return (0);
	*type = 0;
	return (1);
}

int	parse_input(t_lem_in *lem)
{
	char	*data;
	size_t	len;
	size_t	pos;
	char	*line;
	int		type;

	data = read_stdin(&len);
	if (!data || len == 0)
		return (free(data), 0);
	pos = 0;
	line = next_line(data, len, &pos);
	if (!parse_ants(lem, line) || !store_line(&lem->input, line))
		return (free(data), 0);
	lem->hash = hash_new(HASH_SIZE);
	if (!lem->hash)
		return (free(data), 0);
	type = 0;
	line = next_line(data, len, &pos);
	while (line)
	{
		if (!dispatch_line(lem, line, &type))
			return (free(data), 0);
		line = next_line(data, len, &pos);
	}
	return (free(data), lem->graph.start_id >= 0 && lem->graph.end_id >= 0);
}
