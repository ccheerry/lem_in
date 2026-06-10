#include "lem_in.h"

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
