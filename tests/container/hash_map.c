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
