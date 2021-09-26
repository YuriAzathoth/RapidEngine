//
// Copyright (c) 2021 Yuriy Zinchenko.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#define TEST_NO_MAIN
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "acutest.h"
#include "container/hash_map.h"
#include "container/strhash.h"

#define SOURCES_COUNT 94
#define SHUFFLES_COUNT 200

typedef unsigned object_t;

HASH_MAP_INIT(object_t)


//// BEGIN
//typedef unsigned long long hash_t;
//
//#define hash_map(object_t) struct hash_map_##object_t
//#define hash_map_node(object_t) struct hash_map_##object_t##_node
//
//#define hash_map_create(object_t, MAP) hash_map_##object_t##_create(MAP)
//#define hash_map_destroy(object_t, MAP) hash_map_##object_t##_destroy(MAP)
//#define hash_map_clear(object_t, MAP) hash_map_##object_t##_clear(MAP)
//#define hash_map_reserve(object_t, MAP, CAPACITY) hash_map_##object_t##_reserve(MAP, CAPACITY)
//#define hash_map_shrink(object_t, MAP) hash_map_##object_t##_shrink(MAP)
//#define hash_map_find(object_t, MAP, KEY) hash_map_##object_t##_find(MAP, KEY)
//#define hash_map_lower_bound(object_t, MAP, KEY) hash_map_##object_t##_lower_bound(MAP, KEY)
//#define hash_map_insert(object_t, MAP, KEY) hash_map_##object_t##_insert(MAP, KEY)
//#define hash_map_erase(object_t, MAP, KEY) hash_map_##object_t##_erase(MAP, KEY)
//
//hash_map_node(object_t)
//{
//	unsigned long long key;
//	object_t value;
//};
//
//hash_map(object_t)
//{
//	hash_map_node(object_t)* nodes;
//	unsigned capacity;
//	unsigned size;
//};
//
//static void hash_map_create(object_t, hash_map(object_t)* map)
//{
//	map->nodes = NULL;
//	map->capacity = 0;
//	map->size = 0;
//}
//
//static void hash_map_destroy(object_t, hash_map(object_t)* map) { free(map->nodes); }
//
//static void hash_map_clear(object_t, hash_map(object_t)* map)
//{
//	hash_map_destroy(object_t, map);
//	hash_map_create(object_t, map);
//}
//
//static hash_map_node(object_t)* hash_map_lower_bound(object_t, hash_map(object_t)* map, hash_t key)
//{
//	hash_map_node(object_t)* node = map->nodes + key % map->capacity;
//	while (node->key && node->key != key)
//		if (node < map->nodes + map->capacity)
//			++node;
//		else
//			node = map->nodes;
//	return node;
//}
//
//static void hash_map_reserve(object_t, hash_map(object_t)* map, unsigned capacity)
//{
//	if (capacity < map->size)
//		return;
//
//	hash_map(object_t) new_map;
//	new_map.size = map->size;
//	new_map.capacity = capacity;
//	new_map.nodes = (hash_map_node(object_t)*)calloc(capacity, sizeof(hash_map_node(object_t)));
//	if (map->nodes)
//	{
//		hash_map_node(object_t)* dst;
//		for (const hash_map_node(object_t)* src = map->nodes; src < map->nodes + map->capacity; ++src)
//			if (src->key)
//			{
//				dst = hash_map_lower_bound(object_t, &new_map, src->key);
//				dst->key = src->key;
//				memcpy(&dst->value, &src->value, sizeof(object_t));
//			}
//	}
//
//	free(map->nodes);
//	map->nodes = new_map.nodes;
//	map->size = new_map.size;
//	map->capacity = new_map.capacity;
//}
//
//static void hash_map_shrink(object_t, hash_map(object_t)* map)
//{
//	if (map->size)
//		hash_map_reserve(object_t, map, map->size);
//	else
//	{
//		free(map->nodes);
//		map->nodes = NULL;
//		map->capacity = 0;
//	}
//}
//
//static hash_map_node(object_t)* hash_map_find(object_t, hash_map(object_t)* map, hash_t key)
//{
//	hash_map_node(object_t)* node = hash_map_lower_bound(object_t, map, key);
//	return node->key ? node : NULL;
//}
//
//static object_t* hash_map_insert(object_t, hash_map(object_t)* map, hash_t key)
//{
//	if (map->size + 1 > map->capacity)
//		hash_map_reserve(object_t, map, map->capacity ? map->capacity * 2 : 2);
//	hash_map_node(object_t)* node = hash_map_lower_bound(object_t, map, key);
//	node->key = key;
//	++map->size;
//	return &node->value;
//}
//
//static void hash_map_erase(object_t, hash_map(object_t)* map, hash_t key)
//{
//	hash_map_node(object_t)* node = hash_map_lower_bound(object_t, map, key);
//	if (node->key)
//	{
//		node->key = 0;
//		node->value = 0;
//		--map->size;
//	}
//}
//// END

static void shuffle(char arrstr[][2], unsigned arrlen, unsigned count)
{
	unsigned pos1, pos2;
	char temp;
	for (unsigned i = 0; i < count; ++i)
	{
		pos1 = rand() % arrlen;
		pos2 = rand() % arrlen;
		pos2 += (pos1 == pos2);
		temp = arrstr[pos1][0];
		arrstr[pos1][0] = arrstr[pos2][0];
		arrstr[pos2][0] = temp;
	}
}

void test_hash_map()
{
	srand(time(0));

	char sources[SOURCES_COUNT][2];
	for (unsigned i = 0; i < SOURCES_COUNT; ++i)
	{
		sources[i][0] = 33 + i;
		sources[i][1] = '\0';
	}
	shuffle(sources, SOURCES_COUNT, SHUFFLES_COUNT);

	hash_map(object_t) hash_map;
	hash_map_create(object_t, &hash_map);

	for (unsigned i = 0; i < SOURCES_COUNT; ++i)
		*hash_map_insert(object_t, &hash_map, strhash(sources[i])) = i;

	const hash_map_node(object_t)* node;
	hash_t hash;
	for (unsigned i = 0; i < SOURCES_COUNT; ++i)
	{
		hash = strhash(sources[i]);
		node = hash_map_find(object_t, &hash_map, hash);
		TEST_ASSERT(node != NULL);
		TEST_ASSERT(node->key == hash);
		TEST_ASSERT(node->value == i);
	}
	TEST_ASSERT(hash_map.size == SOURCES_COUNT);

	char temp[SOURCES_COUNT][2];
	memcpy(temp, sources, sizeof(temp));

	shuffle(sources, SOURCES_COUNT, SHUFFLES_COUNT);
	for (unsigned i = 0; i < SOURCES_COUNT; ++i)
	{
		hash = strhash(sources[i]);
		hash_map_erase(object_t, &hash_map, hash);
		TEST_ASSERT(hash_map_find(object_t, &hash_map, hash) == NULL);
	}

	TEST_ASSERT(hash_map.size == 0);

	hash_map_destroy(object_t, &hash_map);
}
