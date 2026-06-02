#include "lem_in.h"

static char	**split_link(char *line)
{
	char	**ends;

	ends = ft_split(line, '-');
	if (!ends || !ends[0] || !ends[1] || ends[2])
		return (free_split(ends), NULL);
	return (ends);
}

int	parse_link(t_lem_in *lem, char *line)
{
	char	**ends;

	(void)lem;
	ends = split_link(line);
	if (!ends)
		return (0);
	printf("[link] from=%s to=%s\n", ends[0], ends[1]);
	free_split(ends);
	return (1);
}
