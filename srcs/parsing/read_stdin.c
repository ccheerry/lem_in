#include "lem_in.h"

static char	*grow_buf(char *buf, size_t *cap)
{
	char	*new_buf;
	size_t	new_cap;

	new_cap = *cap * 2;
	new_buf = malloc(new_cap);
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	ft_memcpy(new_buf, buf, *cap);
	free(buf);
	*cap = new_cap;
	return (new_buf);
}

char	*read_stdin(size_t *out_len)
{
	char	*buf;
	size_t	cap;
	size_t	len;
	ssize_t	ret;

	cap = READ_BUF;
	len = 0;
	buf = malloc(cap);
	if (!buf)
		return (NULL);
	ret = read(0, buf + len, READ_BUF);
	while (ret > 0)
	{
		len += ret;
		if (len + READ_BUF > cap)
		{
			buf = grow_buf(buf, &cap);
			if (!buf)
				return (NULL);
		}
		ret = read(0, buf + len, READ_BUF);
	}
	if (ret < 0)
		return (free(buf), NULL);
	if (out_len)
		*out_len = len;
	return (buf);
}
