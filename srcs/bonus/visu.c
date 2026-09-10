/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:55:52 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/10 14:56:10 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

/* Doubles the buffer when the next chunk would not fit. */
static char	*grow(char *buf, size_t *cap, size_t len)
{
	char	*bigger;

	if (len + READ_CHUNK <= *cap)
		return (buf);
	bigger = malloc(*cap * 2);
	if (!bigger)
		return (free(buf), NULL);
	ft_memcpy(bigger, buf, len);
	free(buf);
	*cap *= 2;
	return (bigger);
}

/* Reads a whole stream into one growable buffer. */
char	*slurp(int fd, size_t *len)
{
	char	*buf;
	size_t	cap;
	ssize_t	r;

	cap = READ_CHUNK;
	*len = 0;
	buf = malloc(cap);
	if (!buf)
		return (NULL);
	r = read(fd, buf, READ_CHUNK);
	while (r > 0)
	{
		*len += (size_t)r;
		buf = grow(buf, &cap, *len);
		if (!buf)
			return (NULL);
		r = read(fd, buf + *len, READ_CHUNK);
	}
	if (r < 0)
		return (free(buf), NULL);
	return (buf);
}

static int	fail(const char *msg, char *a, char *b)
{
	free(a);
	free(b);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	return (1);
}

static int	load_template(t_page *p)
{
	int	fd;

	fd = open(TEMPLATE, O_RDONLY);
	if (fd < 0)
		return (0);
	p->tpl = slurp(fd, &p->tlen);
	close(fd);
	return (p->tpl != NULL && p->tlen > 0);
}

int	main(void)
{
	t_page	p;
	t_out	o;

	ft_memset(&p, 0, sizeof(t_page));
	o.len = 0;
	p.data = slurp(STDIN_FILENO, &p.dlen);
	if (!p.data || p.dlen == 0)
		return (fail(NO_DATA, p.data, NULL));
	if (!load_template(&p))
		return (fail(NO_TPL, p.data, p.tpl));
	o.fd = open(PAGE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (o.fd < 0 || !write_page(&o, &p))
		return (fail(NO_TPL, p.data, p.tpl));
	close(o.fd);
	free(p.data);
	free(p.tpl);
	write(STDOUT_FILENO, DONE, sizeof(DONE) - 1);
	return (0);
}
