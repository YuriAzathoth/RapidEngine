/*
	RapidEngine - Fast and cache-friendly C game engine
	Copyright (C) 2021-2022 Yuriy Zinchenko

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>
#include <string.h>

typedef unsigned long long hash_t;

#define hash_map(VALUE_T) struct hash_map_##VALUE_T
#define hash_map_node(VALUE_T) struct hash_map_##VALUE_T##_node

#define hash_map_begin(VALUE_T, MAP) (MAP)->nodes
#define hash_map_end(VALUE_T, MAP) ((MAP)->nodes + (MAP)->capacity)
#define hash_map_first(VALUE_T, MAP) (MAP)->nodes
#define hash_map_last(VALUE_T, MAP) ((MAP)->nodes + (MAP)->capacity - 1)
#define hash_map_foreach(VALUE_T, MAP, IT) for (const hash_map_node(VALUE_T)* src = hash_map_begin(VALUE_T, MAP); src < hash_map_end(VALUE_T, MAP); ++src)

#define hash_map_create(VALUE_T, MAP) hash_map_##VALUE_T##_create(MAP)
#define hash_map_destroy(VALUE_T, MAP) hash_map_##VALUE_T##_destroy(MAP)
#define hash_map_clear(VALUE_T, MAP) hash_map_##VALUE_T##_clear(MAP)
#define hash_map_reserve(VALUE_T, MAP, CAPACITY) hash_map_##VALUE_T##_reserve(MAP, CAPACITY)
#define hash_map_shrink(VALUE_T, MAP) hash_map_##VALUE_T##_shrink(MAP)
#define hash_map_find(VALUE_T, MAP, KEY) hash_map_##VALUE_T##_find(MAP, KEY)
#define hash_map_lower_bound(VALUE_T, MAP, KEY) hash_map_##VALUE_T##_lower_bound(MAP, KEY)
#define hash_map_insert(VALUE_T, MAP, KEY) hash_map_##VALUE_T##_insert(MAP, KEY)
#define hash_map_erase(VALUE_T, MAP, KEY) hash_map_##VALUE_T##_erase(MAP, KEY)

#define HASH_MAP_DECL(VALUE_T)																				\
hash_map_node(VALUE_T)																						\
{																											\
	unsigned long long key;																					\
	VALUE_T value;																							\
};																											\
																											\
hash_map(VALUE_T)																							\
{																											\
	hash_map_node(VALUE_T)* nodes;																			\
	unsigned capacity;																						\
	unsigned size;																							\
};

#define HASH_MAP_FUNC(VALUE_T)																				\
inline static void hash_map_create(VALUE_T, hash_map(VALUE_T)* map)											\
{																											\
	map->nodes = NULL;																						\
	map->capacity = 0;																						\
	map->size = 0;																							\
}																											\
																											\
inline static void hash_map_destroy(VALUE_T, hash_map(VALUE_T)* map) { free(map->nodes); }					\
																											\
inline static void hash_map_clear(VALUE_T, hash_map(VALUE_T)* map)											\
{																											\
	hash_map_destroy(VALUE_T, map);																			\
	hash_map_create(VALUE_T, map);																			\
}																											\
																											\
inline static hash_map_node(VALUE_T)* hash_map_lower_bound(VALUE_T, hash_map(VALUE_T)* map, hash_t key)		\
{																											\
	hash_map_node(VALUE_T)* node = map->nodes + key % map->capacity;										\
	while (node->key && node->key != key)																	\
		if (node < hash_map_last(VALUE_T, map))																\
			++node;																							\
		else																								\
			node = map->nodes;																				\
	return node;																							\
}																											\
																											\
inline static void hash_map_reserve(VALUE_T, hash_map(VALUE_T)* map, unsigned capacity)						\
{																											\
	if (capacity < map->size)																				\
		return;																								\
																											\
	hash_map(VALUE_T) new_map;																				\
	new_map.size = map->size;																				\
	new_map.capacity = capacity;																			\
	new_map.nodes = (hash_map_node(VALUE_T)*)calloc(capacity, sizeof(hash_map_node(VALUE_T)));				\
	if (map->nodes)																							\
	{																										\
		hash_map_node(VALUE_T)* dst;																		\
		hash_map_foreach(VALUE_T, map, it)																	\
			if (src->key)																					\
			{																								\
				dst = hash_map_lower_bound(VALUE_T, &new_map, src->key);									\
				dst->key = src->key;																		\
				memcpy(&dst->value, &src->value, sizeof(VALUE_T));											\
			}																								\
	}																										\
																											\
	free(map->nodes);																						\
	map->nodes = new_map.nodes;																				\
	map->size = new_map.size;																				\
	map->capacity = new_map.capacity;																		\
}																											\
																											\
inline static void hash_map_shrink(VALUE_T, hash_map(VALUE_T)* map)											\
{																											\
	if (map->size)																							\
		hash_map_reserve(VALUE_T, map, map->size);															\
	else																									\
	{																										\
		free(map->nodes);																					\
		map->nodes = NULL;																					\
		map->capacity = 0;																					\
	}																										\
}																											\
																											\
inline static hash_map_node(VALUE_T)* hash_map_find(VALUE_T, hash_map(VALUE_T)* map, hash_t key)			\
{																											\
	hash_map_node(VALUE_T)* node = hash_map_lower_bound(VALUE_T, map, key);									\
	return node->key ? node : NULL;																			\
}																											\
																											\
inline static VALUE_T* hash_map_insert(VALUE_T, hash_map(VALUE_T)* map, hash_t key)							\
{																											\
	if (map->size + 1 > map->capacity)																		\
		hash_map_reserve(VALUE_T, map, map->capacity ? map->capacity * 2 : 2);								\
	hash_map_node(VALUE_T)* node = hash_map_lower_bound(VALUE_T, map, key);									\
	node->key = key;																						\
	++map->size;																							\
	return &node->value;																					\
}																											\
																											\
inline static void hash_map_erase(VALUE_T, hash_map(VALUE_T)* map, hash_t key)								\
{																											\
	hash_map_node(VALUE_T)* node = hash_map_lower_bound(VALUE_T, map, key);									\
	if (node->key)																							\
	{																										\
		node->key = 0;																						\
		--map->size;																						\
	}																										\
}

#define HASH_MAP_INIT(VALUE_T) HASH_MAP_DECL(VALUE_T) HASH_MAP_FUNC(VALUE_T)

#endif // HASH_MAP_H
