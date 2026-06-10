#include "lem_in.h"

void	error_exit(t_lem_in *lem)
{
	free_all(lem);
	write(STDERR_FILENO, "ERROR\n", 6);
	exit(1);
}
