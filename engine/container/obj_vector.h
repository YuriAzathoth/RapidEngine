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

#ifndef OBJ_VECTOR_H
#define OBJ_VECTOR_H

#include <stdlib.h>

#define obj_vector(VALUE_T) struct obj_vector_##VALUE_T

#define obj_vector_begin(VECTOR) (VECTOR)->data
#define obj_vector_end(VECTOR) ((VECTOR)->data + (VECTOR)->size)
#define obj_vector_front(VECTOR) (VECTOR)->data
#define obj_vector_back(VECTOR) obj_vector_end(VECTOR) - 1
#define obj_vector_get(VECTOR, POSITION) ((VECTOR)->data + POSITION)

#define obj_vector_create(VALUE_T, VECTOR) obj_vector_##VALUE_T##_create(VECTOR)
#define obj_vector_destroy(VALUE_T, VECTOR) obj_vector_##VALUE_T##_destroy(VECTOR)
#define obj_vector_clear(VALUE_T, VECTOR) obj_vector_##VALUE_T##_clear(VECTOR)
#define obj_vector_reserve(VALUE_T, VECTOR, CAPACITY) obj_vector_##VALUE_T##_reserve(VECTOR, CAPACITY)
#define obj_vector_shrink(VALUE_T, VECTOR) obj_vector_##VALUE_T##_shrink(VECTOR)
#define obj_vector_push(VALUE_T, VECTOR) obj_vector_##VALUE_T##_push(VECTOR)
#define obj_vector_pop(VALUE_T, VECTOR) obj_vector_##VALUE_T##_pop(VECTOR)
#define obj_vector_erase(VALUE_T, VECTOR, POSITION) obj_vector_##VALUE_T##_erase(VECTOR, POSITION)

#define VECTOR_DECL(VALUE_T)																					\
obj_vector(VALUE_T)																								\
{																												\
	VALUE_T* data;																								\
	unsigned capacity;																							\
	unsigned size;																								\
};

#define VECTOR_FUNC(VALUE_T)																					\
inline static void obj_vector_create(VALUE_T, obj_vector(VALUE_T)* vector)										\
{																												\
	vector->data = NULL;																						\
	vector->capacity = 0;																						\
	vector->size = 0;																							\
}																												\
																												\
inline static void obj_vector_destroy(VALUE_T, obj_vector(VALUE_T)* vector) { free(vector->data); }				\
																												\
inline static void obj_vector_clear(VALUE_T, obj_vector(VALUE_T)* vector)										\
{																												\
	obj_vector_destroy(VALUE_T, vector);																		\
	obj_vector_create(VALUE_T, vector);																			\
}																												\
																												\
inline static void obj_vector_reserve(VALUE_T, obj_vector(VALUE_T)* vector, unsigned capacity)					\
{																												\
	if (!capacity)																								\
		capacity = 1;																							\
	if (capacity != vector->capacity && capacity > vector->size)												\
	{																											\
		vector->data = realloc(vector->data, capacity * sizeof(VALUE_T));										\
		vector->capacity = capacity;																			\
	}																											\
}																												\
																												\
inline static void obj_vector_shrink(VALUE_T, obj_vector(VALUE_T)* vector)										\
{																												\
	if (vector->size)																							\
		vector->data = realloc(vector->data, vector->size * sizeof(VALUE_T));									\
	else																										\
		vector->data = NULL;																					\
	vector->capacity = vector->size;																			\
}																												\
																												\
inline static VALUE_T* obj_vector_push(VALUE_T, obj_vector(VALUE_T)* vector)									\
{																												\
	if (vector->size + 1 > vector->capacity)																	\
		obj_vector_reserve(VALUE_T, vector, vector->capacity * 2);												\
	VALUE_T* obj = obj_vector_end(vector);																		\
	++vector->size;																								\
	return obj;																									\
}																												\
																												\
inline static void obj_vector_pop(VALUE_T, obj_vector(VALUE_T)* vector) { --vector->size; }						\
																												\
inline static void obj_vector_erase(VALUE_T, obj_vector(VALUE_T)* vector, unsigned position)					\
{																												\
	memcpy(vector->data + position, obj_vector_back(vector), sizeof(VALUE_T));									\
	--vector->size;																								\
}

#define VECTOR_INIT(VALUE_T) VECTOR_DECL(VALUE_T) VECTOR_FUNC(VALUE_T)

#endif // OBJ_VECTOR_H
