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
