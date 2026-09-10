/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:20:40 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/10 14:55:57 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

int	flush_out(t_out *o)
{
	ssize_t	w;
	size_t	off;

	off = 0;
	while (off < o->len)
	{
		w = write(o->fd, o->buf + off, o->len - off);
		if (w <= 0)
			return (0);
		off += (size_t)w;
	}
	o->len = 0;
	return (1);
}

void	put(t_out *o, const char *s, size_t n)
{
	size_t	take;

	while (n > 0)
	{
		if (o->len == OUT_BUF && !flush_out(o))
			return ;
		take = OUT_BUF - o->len;
		if (take > n)
			take = n;
		ft_memcpy(o->buf + o->len, s, take);
		o->len += take;
		s += take;
		n -= take;
	}
}

/* Escapes the simulation into a JS string; "/" breaks any </script inside. */
void	put_escaped(t_out *o, const char *s, size_t n)
{
	size_t	i;
	char	c;

	i = 0;
	while (i < n)
	{
		c = s[i++];
		if (c == '"' || c == '\\' || c == '/')
		{
			put(o, "\\", 1);
			put(o, &c, 1);
		}
		else if (c == '\n')
			put(o, "\\n", 2);
		else if (c == '\r')
			put(o, "\\r", 2);
		else
			put(o, &c, 1);
	}
}

static char	*find_body(char *tpl, size_t tlen)
{
	size_t	i;

	i = 0;
	while (i + 7 <= tlen)
	{
		if (ft_strncmp(tpl + i, "</body>", 7) == 0)
			return (tpl + i);
		i++;
	}
	return (NULL);
}

/* Copies the page and drops a load() call for this run right before </body>. */
int	write_page(t_out *o, t_page *p)
{
	char	*split;

	split = find_body(p->tpl, p->tlen);
	if (!split)
		return (0);
	put(o, p->tpl, (size_t)(split - p->tpl));
	put(o, OPEN_HEAD, sizeof(OPEN_HEAD) - 1);
	put_escaped(o, p->data, p->dlen);
	put(o, OPEN_TAIL, sizeof(OPEN_TAIL) - 1);
	put(o, split, p->tlen - (size_t)(split - p->tpl));
	return (flush_out(o));
}
