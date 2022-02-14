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

#ifndef OCTREE_H
#define OCTREE_H

#include <cglm/types.h>
#include "container/obj_vector.h"

enum octree_node_type
{
	ONT_NXNYNZ,
	ONT_PXNYNZ,
	ONT_NXPYNZ,
	ONT_PXPYNZ,
	ONT_NXNYPZ,
	ONT_PXNYPZ,
	ONT_NXPYPZ,
	ONT_PXPYPZ,
	OCTREE_NODES_COUNT
};

struct octree_data
{
	vec3 box[2];
	unsigned model_id;
};

typedef struct octree_data octree_data_t;
VECTOR_DECL(octree_data_t)
typedef obj_vector(octree_data_t) octree_leaf_t;

struct octree_node
{
	unsigned children[OCTREE_NODES_COUNT];
	vec3 box[2];
	unsigned id;
	unsigned parent;
	int leaf;
};

typedef struct octree_node octree_node_t;
VECTOR_DECL(octree_leaf_t)
VECTOR_DECL(octree_node_t)

struct octree
{
	obj_vector(octree_node_t) nodes;
	obj_vector(octree_leaf_t) leafs;
	unsigned max_objects;
};

void octree_create(struct octree* octree, unsigned capacity, unsigned max_objects, vec3 box[2]);
void octree_destroy(struct octree* octree);

void octree_attach(struct octree* octree, vec3 box[2], unsigned model_id);

#endif // OCTREE_H
