/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acerezo- <acerezo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 20:59:16 by ravazque          #+#    #+#             */
/*   Updated: 2026/09/08 17:49:53 by acerezo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "../lib/libft.h"

# define HASH_SIZE 1024
# define ROOMS_INIT_CAP 64
# define START_ROOM 1
# define END_ROOM 2
# define INIT_CAP 64
# define READ_BUF 4096
# define LINKS_INIT_CAP 64

# define ARGS_ERR \
	"Error!\nCompile the executable without any arguments.\n"

# define ERR \
	"ERROR\n"

/*
** A node in the ant farm graph.
** id is the numeric index;
** after node-splitting it maps to in = id*2, out = id*2+1.
*/
typedef struct s_room
{
	char			*name;
	int				x;
	int				y;
	int				id;
}	t_room;

typedef struct s_link
{
	int		from;
	int		to;
}	t_link;

/*
** Hash table for room lookup by name during parsing.
*/
typedef struct s_hash_entry
{
	char				*key;
	t_room				*room;
	struct s_hash_entry	*next;
}	t_hash_entry;

typedef struct s_hash_table
{
	t_hash_entry	**buckets;
	int				size;
}	t_hash_table;

typedef struct s_path
{
	int				*rooms;
	int				len;
	int				ants_assigned;
	int				first_ant;
	struct s_path	*next;
}	t_path;

/*
** A directed edge in the residual graph. Internal edges come in pairs:
** a forward edge with the capacity and a reverse edge starting at 0.
*/
typedef struct s_edge
{
	int				to;
	int				cap;
	int				is_rev;
	struct s_edge	*rev;
	struct s_edge	*next;
}	t_edge;

/*
** Scratch buffers reused across every BFS pass: parent edge that reached each
** node, the FIFO queue, and the visited marker.
*/
typedef struct s_bfs
{
	t_edge	**parent;
	int		*queue;
	char	*visited;
}	t_bfs;

/*
** Node-split graph as adjacency lists (O(V + E) memory). Each room becomes two
** nodes, in (id*2) and out (id*2+1), so num_nodes = num_rooms*2.
*/
typedef struct s_graph
{
	t_room			**rooms;
	t_link			*links;
	int				num_rooms;
	int				rooms_cap;
	int				num_links;
	int				links_cap;
	int				start_id;
	int				end_id;
	t_edge			**adj;
	int				num_nodes;
}	t_graph;

typedef struct s_input
{
	char			**lines;
	int				count;
	int				capacity;
}	t_input;

/*
** Growable output buffer: everything is appended here and flushed with a
** single write() at the end, so large maps avoid thousands of tiny writes.
*/
typedef struct s_buf
{
	char			*data;
	size_t			len;
	size_t			cap;
}	t_buf;

typedef struct s_lem_in
{
	int				num_ants;
	t_graph			graph;
	t_hash_table	*hash;
	t_path			*paths;
	int				num_paths;
	t_input			input;
}	t_lem_in;

// =[ parsing ]============================================================= //

char			*read_stdin(size_t *out_len);
int				parse_input(t_lem_in *lem);
int				parse_room(t_lem_in *lem, char *line, int type);
int				parse_link(t_lem_in *lem, char *line);
int				store_line(t_input *input, char *line);

// =[ graph ]=============================================================== //

void			graph_init(t_lem_in *lem);
void			node_split(t_lem_in *lem);
int				add_edge(t_graph *graph, int from, int to, int cap);
t_hash_table	*hash_new(int size);
void			hash_insert(t_hash_table *ht, char *key, t_room *room);
t_room			*hash_lookup(t_hash_table *ht, char *key);
void			hash_free(t_hash_table *ht);

// =[ solver ]============================================================== //

int				algorithm(t_lem_in *lem);
t_path			*extract_paths(t_lem_in *lem);
int				select_paths(t_lem_in *lem);
int				calc_turns(t_path *paths, int num_paths, int num_ants);
void			assign_counts(t_lem_in *lem, t_path **arr, int k);

// =[ simulation ]========================================================== //

void			assign_ants(t_lem_in *lem);
void			simulate(t_lem_in *lem);
int				buf_append(t_buf *b, const char *s, size_t n);
int				buf_putnbr(t_buf *b, int n);

// =[ utils ]=============================================================== //

void			error_exit(t_lem_in *lem);
void			free_split(char **arr);
void			free_all(t_lem_in *lem);

// ========================================================================= //

#endif
