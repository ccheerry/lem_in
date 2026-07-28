/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:24 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 21:36:24 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	error_exit(t_lem_in *lem)
{
	free_all(lem);
	write(STDERR_FILENO, ERR, 6);
	exit(1);
}

int	main(int argc, char *argv[])
{
	t_lem_in	lem;

	(void)argv;
	if (argc != 1)
		return (write(STDERR_FILENO, ARGS_ERR, 53), 1);
	ft_memset(&lem, 0, sizeof(t_lem_in));
	lem.graph.start_id = -1;
	lem.graph.end_id = -1;
	if (!parse_input(&lem))
		error_exit(&lem);
	graph_init(&lem);
	node_split(&lem);
	if (!algorithm(&lem))
		error_exit(&lem);
	select_paths(&lem);
	assign_ants(&lem);
	simulate(&lem);
	free_all(&lem);
	return (0);
}
