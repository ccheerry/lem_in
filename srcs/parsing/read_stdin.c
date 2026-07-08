/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:58:53 by ravazque          #+#    #+#             */
/*   Updated: 2026/06/15 20:59:33 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static char	*grow_buf(char *buf, size_t *cap, size_t len)
{
	char	*new_buf;

	if (len + READ_BUF <= *cap)
		return (buf);
	new_buf = malloc(*cap * 2);
	if (!new_buf)
		return (free(buf), NULL);
	ft_memcpy(new_buf, buf, *cap);
	free(buf);
	*cap *= 2;
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
		buf = grow_buf(buf, &cap, len);
		if (!buf)
			return (NULL);
		ret = read(0, buf + len, READ_BUF);
	}
	if (ret < 0)
		return (free(buf), NULL);
	if (out_len)
		*out_len = len;
	return (buf);
}
