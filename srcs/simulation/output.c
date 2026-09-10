/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:45 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/09 12:10:04 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static int	buf_grow(t_buf *b, size_t need)
{
	char	*bigger;
	size_t	cap;

	if (b->len + need <= b->cap)
		return (1);
	cap = b->cap;
	if (cap == 0)
		cap = OUT_BUF;
	while (b->len + need > cap)
		cap *= 2;
	bigger = malloc(cap);
	if (!bigger)
		return (0);
	if (b->data)
	{
		ft_memcpy(bigger, b->data, b->len);
		free(b->data);
	}
	b->data = bigger;
	b->cap = cap;
	return (1);
}

int	buf_flush(t_buf *b)
{
	ssize_t	written;
	size_t	off;

	off = 0;
	while (off < b->len)
	{
		written = write(STDOUT_FILENO, b->data + off, b->len - off);
		if (written <= 0)
			return (0);
		off += (size_t)written;
	}
	b->len = 0;
	return (1);
}

/* Flushing once the buffer is full keeps memory flat on huge simulations. */
int	buf_append(t_buf *b, const char *s, size_t n)
{
	if (b->len + n > OUT_BUF && !buf_flush(b))
		return (0);
	if (!buf_grow(b, n))
		return (0);
	ft_memcpy(b->data + b->len, s, n);
	b->len += n;
	return (1);
}

int	buf_putnbr(t_buf *b, int n)
{
	char	tmp[12];
	int		i;

	i = 12;
	if (n == 0)
		tmp[--i] = '0';
	while (n > 0)
	{
		tmp[--i] = '0' + (n % 10);
		n /= 10;
	}
	return (buf_append(b, tmp + i, 12 - i));
}
