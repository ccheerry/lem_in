#include "lem_in.h"

static unsigned long	hash_key(const char *key)
{
	unsigned long	hash;
	int				i;

	hash = 5381;
	i = 0;
	while (key[i])
	{
		hash = ((hash << 5) + hash) + (unsigned char)key[i];
		i++;
	}
	return (hash);
}

t_hash_table	*hash_new(int size)
{
	t_hash_table	*ht;

	ht = malloc(sizeof(t_hash_table));
	if (!ht)
		return (NULL);
	ht->buckets = ft_calloc(size, sizeof(t_hash_entry *));
	if (!ht->buckets)
		return (free(ht), NULL);
	ht->size = size;
	return (ht);
}

void	hash_insert(t_hash_table *ht, char *key, t_room *room)
{
	t_hash_entry	*entry;
	unsigned long	idx;

	if (!ht)
		return ;
	entry = malloc(sizeof(t_hash_entry));
	if (!entry)
		return ;
	entry->key = key;
	entry->room = room;
	idx = hash_key(key) % ht->size;
	entry->next = ht->buckets[idx];
	ht->buckets[idx] = entry;
}

t_room	*hash_lookup(t_hash_table *ht, char *key)
{
	t_hash_entry	*entry;
	unsigned long	idx;

	if (!ht || !key)
		return (NULL);
	idx = hash_key(key) % ht->size;
	entry = ht->buckets[idx];
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
			return (entry->room);
		entry = entry->next;
	}
	return (NULL);
}

void	hash_free(t_hash_table *ht)
{
	(void)ht;
}
