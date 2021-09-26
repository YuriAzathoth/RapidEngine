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
