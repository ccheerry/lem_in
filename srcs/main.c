#include "lem_in.h"

int	main(void)
{
	t_lem_in	lem;

	ft_memset(&lem, 0, sizeof(t_lem_in));
	if (!parse_input(&lem))
		error_exit(&lem);
	graph_init(&lem);
	node_split(&lem);
	if (!algo(&lem))
		error_exit(&lem);
	select_paths(&lem);
	assign_ants(&lem);
	simulate(&lem);
	free_all(&lem);
	return (0);
}
