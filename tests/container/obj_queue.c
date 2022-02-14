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
#include "container/obj_queue.h"

typedef int object_t;
QUEUE_INIT(object_t)

void test_obj_queue()
{
	obj_queue(object_t) obj_queue;
	obj_queue_create(object_t, &obj_queue);

	const int num[] = { 0, 1, 2, 3, 4 };
	for (int i = 0; i < sizeof(num) / sizeof(int); ++i)
		*obj_queue_push(object_t, &obj_queue) = num[i];
	TEST_ASSERT(obj_queue.data != NULL);
	TEST_ASSERT(obj_queue.head == obj_queue.data);
	TEST_ASSERT(obj_queue.tail > obj_queue.head);
	TEST_ASSERT(obj_queue.capacity == 8);
	TEST_ASSERT(obj_queue.size == 5);

	TEST_ASSERT(*obj_queue_front(&obj_queue) == 0);
	obj_queue_pop(object_t, &obj_queue);
	TEST_ASSERT(*obj_queue_front(&obj_queue) == 1);
	obj_queue_pop(object_t, &obj_queue);
	TEST_ASSERT(obj_queue.data != NULL);
	TEST_ASSERT(obj_queue.capacity == 8);
	TEST_ASSERT(obj_queue.size == 3);

	*obj_queue_push(object_t, &obj_queue) = num[3];
	*obj_queue_push(object_t, &obj_queue) = num[4];
	TEST_ASSERT(obj_queue.data != NULL);
	TEST_ASSERT(obj_queue.capacity == 8);
	TEST_ASSERT(obj_queue.size == 5);

	TEST_ASSERT(*(int*)obj_queue_front(&obj_queue) == 2);
	obj_queue_pop(object_t, &obj_queue);
	TEST_ASSERT(*(int*)obj_queue_front(&obj_queue) == 3);
	obj_queue_pop(object_t, &obj_queue);
	TEST_ASSERT(*(int*)obj_queue_front(&obj_queue) == 4);
	obj_queue_pop(object_t, &obj_queue);
	TEST_ASSERT(*(int*)obj_queue_front(&obj_queue) == 3);
	obj_queue_pop(object_t, &obj_queue);
	TEST_ASSERT(*(int*)obj_queue_front(&obj_queue) == 4);
	obj_queue_pop(object_t, &obj_queue);
	TEST_ASSERT(obj_queue.data != NULL);
	TEST_ASSERT(obj_queue.head == obj_queue.tail);
	TEST_ASSERT(obj_queue.capacity == 8);
	TEST_ASSERT(obj_queue.size == 0);

	obj_queue_destroy(object_t, &obj_queue);
}
