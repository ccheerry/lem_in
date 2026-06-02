#include "lem_in.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_all(t_lem_in *lem)
{
	(void)lem;
}
