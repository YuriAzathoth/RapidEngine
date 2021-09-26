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

#ifndef OBJ_QUEUE_H
#define OBJ_QUEUE_H

#include <stdlib.h>

#define obj_queue(VALUE_T) struct obj_queue_##VALUE_T

#define obj_queue_begin(QUEUE) (QUEUE)->data
#define obj_queue_end(QUEUE) ((QUEUE)->data + (QUEUE)->capacity)
#define obj_queue_front(QUEUE) (QUEUE)->head
#define obj_queue_back(QUEUE) ((QUEUE)->tail - 1)

#define obj_queue_create(VALUE_T, QUEUE) obj_queue_##VALUE_T##_create(QUEUE)
#define obj_queue_destroy(VALUE_T, QUEUE) obj_queue_##VALUE_T##_destroy(QUEUE)
#define obj_queue_clear(VALUE_T, QUEUE) obj_queue_##VALUE_T##_clear(QUEUE)
#define obj_queue_reserve(VALUE_T, QUEUE, CAPACITY) obj_queue_##VALUE_T##_reserve(QUEUE, CAPACITY)
#define obj_queue_shrink(VALUE_T, QUEUE) obj_queue_##VALUE_T##_shrink(QUEUE)
#define obj_queue_push(VALUE_T, QUEUE) obj_queue_##VALUE_T##_push(QUEUE)
#define obj_queue_pop(VALUE_T, QUEUE) obj_queue_##VALUE_T##_pop(QUEUE)

#define QUEUE_DECL(VALUE_T)																						\
struct obj_queue_##VALUE_T																						\
{																												\
	VALUE_T* data;																								\
	VALUE_T* head;																								\
	VALUE_T* tail;																								\
	unsigned capacity;																							\
	unsigned size;																								\
};

#define QUEUE_FUNC(VALUE_T)																						\
inline static void obj_queue_create(VALUE_T, obj_queue(VALUE_T)* queue)											\
{																												\
	queue->data = NULL;																							\
	queue->head = NULL;																							\
	queue->tail = NULL;																							\
	queue->capacity = 0;																						\
	queue->size = 0;																							\
}																												\
																												\
inline static void obj_queue_destroy(VALUE_T, obj_queue(VALUE_T)* queue) { free(queue->data); }					\
																												\
inline static void obj_queue_clear(VALUE_T, obj_queue(VALUE_T)* queue)											\
{																												\
	obj_queue_destroy(VALUE_T, queue);																			\
	obj_queue_create(VALUE_T, queue);																			\
}																												\
																												\
inline static void obj_queue_reserve(VALUE_T, obj_queue(VALUE_T)* queue, unsigned capacity)						\
{																												\
	VALUE_T* data = (VALUE_T*)malloc(capacity * sizeof(VALUE_T));												\
	VALUE_T* tail = data;																						\
	if (queue->data)																							\
	{																											\
		const VALUE_T* src;																						\
		if (queue->head < queue->tail)																			\
			for (src = queue->head; src < queue->tail; ++src, ++tail)											\
				 memcpy(tail, src, sizeof(VALUE_T));															\
		else																									\
		{																										\
			for (src = queue->head; src < queue->data + queue->capacity; ++src, ++tail)							\
				 memcpy(tail, src, sizeof(VALUE_T));															\
			for (src = queue->data; src < queue->tail; ++src, ++tail)											\
				 memcpy(tail, src, sizeof(VALUE_T));															\
		}																										\
	}																											\
	free(queue->data);																							\
	queue->data = data;																							\
	queue->head = data;																							\
	queue->tail = tail;																							\
	queue->capacity = capacity;																					\
}																												\
																												\
inline static void obj_queue_shrink(VALUE_T, obj_queue(VALUE_T)* queue)											\
{																												\
	obj_queue_reserve(VALUE_T, queue, queue->size);																\
}																												\
																												\
inline static VALUE_T* obj_queue_push(VALUE_T, obj_queue(VALUE_T)* queue)										\
{																												\
	if (!queue->data)																							\
		obj_queue_reserve(VALUE_T, queue, 1);																	\
	else if (queue->size + 1 > queue->capacity)																	\
		obj_queue_reserve(VALUE_T, queue, queue->capacity * 2);															\
	if (queue->tail >= obj_queue_end(queue))																	\
		queue->tail = obj_queue_begin(queue);																	\
	++queue->size;																								\
	VALUE_T* ret = queue->tail;																					\
	++queue->tail;																								\
	return ret;																									\
}																												\
																												\
inline static void obj_queue_pop(VALUE_T, obj_queue(VALUE_T)* queue)											\
{																												\
	++queue->head;																								\
	if (queue->head > obj_queue_end(queue))																		\
		queue->head = obj_queue_begin(queue);																	\
	--queue->size;																								\
}

#define QUEUE_INIT(VALUE_T) QUEUE_DECL(VALUE_T)	QUEUE_FUNC(VALUE_T)

#endif // OBJ_QUEUE_H
