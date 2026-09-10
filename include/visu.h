/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:20:43 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/10 14:55:39 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VISU_H
# define VISU_H

# include "../lib/libft.h"
# include <fcntl.h>

# define TEMPLATE "visualizer.html"
# define PAGE "lem-in-visu.html"
# define READ_CHUNK 65536
# define OUT_BUF 65536

# define OPEN_HEAD "<script>window.load(\""
# define OPEN_TAIL "\");</script>\n"

# define NO_TPL \
	"Error!\nvisualizer.html not found. Run this from the project root.\n"

# define NO_DATA \
	"Error!\nNothing on stdin. Use: ./lem-in < map | ./visu\n"

# define DONE "lem-in-visu.html written. Open it in your browser.\n"

/* The visualizer page plus the simulation to be injected into it. */
typedef struct s_page
{
	char	*tpl;
	size_t	tlen;
	char	*data;
	size_t	dlen;
}	t_page;

/* Buffered writer: one write() per OUT_BUF bytes instead of one per byte. */
typedef struct s_out
{
	int		fd;
	size_t	len;
	char	buf[OUT_BUF];
}	t_out;

char	*slurp(int fd, size_t *len);
int		flush_out(t_out *o);
void	put(t_out *o, const char *s, size_t n);
void	put_escaped(t_out *o, const char *s, size_t n);
int		write_page(t_out *o, t_page *p);

#endif
