#include "lem_in.h"

t_hash_table	*hash_new(int size)
{
	(void)size;
	return (NULL);
}

void	hash_insert(t_hash_table *ht, char *key, t_room *room)
{
	(void)ht;
	(void)key;
	(void)room;
}

t_room	*hash_lookup(t_hash_table *ht, char *key)
{
	(void)ht;
	(void)key;
	return (NULL);
}

void	hash_free(t_hash_table *ht)
{
	(void)ht;
}
