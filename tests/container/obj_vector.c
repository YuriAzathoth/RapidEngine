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
#include "acutest.h"
#include "container/obj_vector.h"

typedef int object_t;
VECTOR_INIT(object_t)

void test_obj_vector()
{
	obj_vector(object_t) vector;
	obj_vector_create(object_t, &vector);

	const int num[] = { 0, 1, 2, 3, 4 };
	for (int i = 0; i < 5; ++i)
		*obj_vector_push(object_t, &vector) = num[i];
	TEST_ASSERT(*obj_vector_get(&vector, 0) == 0);
	TEST_ASSERT(*obj_vector_get(&vector, 1) == 1);
	TEST_ASSERT(*obj_vector_get(&vector, 2) == 2);
	TEST_ASSERT(*obj_vector_get(&vector, 3) == 3);
	TEST_ASSERT(*obj_vector_get(&vector, 4) == 4);
	TEST_ASSERT(vector.data != NULL);
	TEST_ASSERT(vector.capacity == 8);
	TEST_ASSERT(vector.size == 5);

	obj_vector_erase(object_t, &vector, 1);
	TEST_ASSERT(*obj_vector_get(&vector, 0) == 0);
	TEST_ASSERT(*obj_vector_get(&vector, 1) == 4);
	TEST_ASSERT(*obj_vector_get(&vector, 2) == 2);
	TEST_ASSERT(*obj_vector_get(&vector, 3) == 3);
	TEST_ASSERT(vector.data != NULL);
	TEST_ASSERT(vector.capacity == 8);
	TEST_ASSERT(vector.size == 4);

	obj_vector_erase(object_t, &vector, 0);
	TEST_ASSERT(*obj_vector_get(&vector, 0) == 3);
	TEST_ASSERT(*obj_vector_get(&vector, 1) == 4);
	TEST_ASSERT(*obj_vector_get(&vector, 2) == 2);
	TEST_ASSERT(vector.data != NULL);
	TEST_ASSERT(vector.capacity == 8);
	TEST_ASSERT(vector.size == 3);

	obj_vector_erase(object_t, &vector, 1);
	TEST_ASSERT(*obj_vector_get(&vector, 0) == 3);
	TEST_ASSERT(*obj_vector_get(&vector, 1) == 2);
	TEST_ASSERT(vector.data != NULL);
	TEST_ASSERT(vector.capacity == 8);
	TEST_ASSERT(vector.size == 2);

	obj_vector_shrink(object_t, &vector);
	TEST_ASSERT(vector.data != NULL);
	TEST_ASSERT(vector.capacity == 2);
	TEST_ASSERT(vector.size == 2);

	obj_vector_erase(object_t, &vector, 0);
	TEST_ASSERT(*obj_vector_get(&vector, 0) == 2);
	TEST_ASSERT(vector.data != NULL);
	TEST_ASSERT(vector.capacity == 2);
	TEST_ASSERT(vector.size == 1);

	obj_vector_erase(object_t, &vector, 0);
	TEST_ASSERT(vector.data != NULL);
	TEST_ASSERT(vector.capacity == 2);
	TEST_ASSERT(vector.size == 0);

	obj_vector_shrink(object_t, &vector);
	TEST_ASSERT(vector.data == NULL);
	TEST_ASSERT(vector.capacity == 0);
	TEST_ASSERT(vector.size == 0);

	obj_vector_destroy(object_t, &vector);
}
