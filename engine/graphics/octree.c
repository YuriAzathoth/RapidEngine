/*
	RapidEngine - Fast and cache-friendly C game engine
	Copyright (C) 2021 Yuriy Zinchenko

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

#include <cglm/box.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include "core/common.h"
#include "octree.h"

VECTOR_FUNC(octree_data_t)
VECTOR_FUNC(octree_leaf_t)
VECTOR_FUNC(octree_node_t)

ENGINE_INLINE void octree_node_init(struct octree_node* node, unsigned id, unsigned parent, int leaf, vec3 box[2])
{
	node->id = id;
	node->parent = parent;
	node->leaf = leaf;
	for (unsigned i = 0; i < 2; ++i)
		glm_vec3_copy(box[i], node->box[i]);
	for (unsigned i = 0; i < OCTREE_NODES_COUNT; ++i)
		node->children[i] = 0;
}

ENGINE_INLINE unsigned get_child_id(vec3 parent_box[2], vec3 child_box[2])
{
	vec3 child_center, parent_center;
	glm_aabb_center(child_box, child_center);
	glm_aabb_center(parent_box, parent_center);
	return ((child_center[0] > parent_center[0])) +
		   ((child_center[1] > parent_center[1]) << 1) +
		   ((child_center[2] > parent_center[2]) << 2);
}

void octree_create(struct octree* octree, unsigned capacity, unsigned max_objects, vec3 box[2])
{
	obj_vector_create(octree_node_t, &octree->nodes);
	obj_vector_create(octree_leaf_t, &octree->leafs);
	obj_vector_reserve(octree_node_t, &octree->nodes, capacity);
	obj_vector_reserve(octree_leaf_t, &octree->leafs, capacity);
	octree->max_objects = max_objects;
	obj_vector_push(octree_leaf_t, &octree->leafs);
	octree_node_init(obj_vector_push(octree_node_t, &octree->nodes), 0, 0, 0, box);
}

void octree_destroy(struct octree* octree)
{
	obj_vector_destroy(octree_node_t, &octree->nodes);
	obj_vector_foreach(octree_leaf_t, &octree->leafs, it)
		obj_vector_destroy(octree_data_t, it);
	obj_vector_destroy(octree_leaf_t, &octree->leafs);
}

void octree_attach(struct octree* octree, vec3 box[2], unsigned model_id)
{
	struct octree_node* node = &octree->nodes[0];
	while (node->leaf == -1)
		node = &octree->nodes[node->children[get_child_id(node->box, box)]];

	octree_leaf_t* leaf = &octree->leafs[node->leaf];
	if (leaf.size >= octree->max_objects)
	{
		const unsigned parent_id = (unsigned)(node - octree->nodes);
		unsigned child_id, child_leaf_id;
		struct octree_node* child_node;
		octree_leaf_t child_leaf;
		for (unsigned i = 0; i < OCTREE_NODES_COUNT; ++i)
		{
			child_node = obj_vector_push(octree_node_t, &octree->nodes);
			child_id = (unsigned)(child_node - octree->nodes);
			child_leaf = obj_vector_push(octree_leaf_t, &octree->leafs);
			child_leaf_id = (unsigned)(child_leaf - octree->leafs);
			octree_node_init(child_node, child_id, parent_id, child_leaf_id, box);
		}
		obj_vector_foreach(octree_data_t, )
		node->leaf = -1;
	}
}
