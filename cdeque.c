#include <stdlib.h>

#include "cdeque.h"

cdeque * cdeque_create(void)
{
	cdeque *queue = malloc(sizeof(cdeque));
	if (queue) {
		queue->arrays = dynarray_create(0);
		queue->arraysize = 4;
		queue->firstempty = 1;
		queue->lastempty = 1;
		queue->count = 0;
		dynarray_add_head(queue->arrays, malloc(queue->arraysize * sizeof(void*)));
	}
	return queue;
}

void cdeque_delete(cdeque * queue)
{
	if (queue) {
		dynarray_for_each(queue->arrays, free);
		dynarray_delete(queue->arrays);
		free(queue);
	}
}

void cdeque_push_front(cdeque * queue, void * data)
{
	unsigned int index;
	if (queue->count == 0) {
		/* Adding the first element */
		index = queue->arraysize / 2;
	}
	else if (queue->firstempty) {
		/* There is an empty array at the beginning */
		index = queue->arraysize - 1;
	}
	else if (queue->front == 0) {
		/* Beginning array is full - add another */
		dynarray_add_head(queue->arrays, malloc(queue->arraysize * sizeof(void*)));
		index = queue->arraysize - 1;
	}
	else {
		index = queue->front - 1;
	}
	((void**)dynarray_get(queue->arrays, 0))[index] = data;
	queue->front = index;
	queue->firstempty = 0;
	if (queue->arrays->count == 1) {
		queue->lastempty = 0;
	}
	queue->count++;
	if (queue->count == 1) {
		queue->back = queue->front;
	}
}

void cdeque_push_back(cdeque * queue, void * data)
{
	unsigned int index;
	if (queue->count == 0) {
		/* Adding the first element */
		index = queue->arraysize / 2;
	}
	else if (queue->lastempty) {
		/* There is an empty array at the end */
		index = 0;
	}
	else if (queue->back == queue->arraysize - 1) {
		/* End array is full - add another */
		dynarray_add_tail(queue->arrays, malloc(queue->arraysize * sizeof(void*)));
		index = 0;
	}
	else {
		index = queue->back + 1;
	}
	((void**)dynarray_get(queue->arrays, queue->arrays->count - 1))[index] = data;
	queue->back = index;
	queue->lastempty = 0;
	if (queue->arrays->count == 1) {
		queue->firstempty = 0;
	}
	queue->count++;
	if (queue->count == 1) {
		queue->front = queue->back;
	}
}

void * cdeque_pop_front(cdeque * queue)
{
	void *data = NULL;
	if (queue->count) {
		if (queue->firstempty) {
			/* There is an empty array at the beginning */
			free(dynarray_remove_head(queue->arrays));
			queue->firstempty = 0;
		}
		data = ((void**)dynarray_get(queue->arrays, 0))[queue->front];
		queue->front++;
		if (queue->front == queue->arraysize) {
			/* First array is now empty */
			queue->firstempty = 1;
			queue->front = 0;
		}
		queue->count--;
		if (queue->count == 1) {
			queue->back = queue->front;
		}
		else if (queue->count == 0 && queue->arrays->count == 2) {
			/* Both arrays are empty - remove either one */
			free(dynarray_remove_head(queue->arrays));
		}
	}
	return data;
}

void * cdeque_pop_back(cdeque * queue)
{
	void *data = NULL;
	if (queue->count) {
		if (queue->lastempty) {
			/* There is an empty array at the end */
			free(dynarray_remove_tail(queue->arrays));
			queue->lastempty = 0;
		}
		data = ((void**)dynarray_get(queue->arrays, queue->arrays->count - 1))[queue->back];
		if (queue->back == 0) {
			/* Last array is now empty */
			queue->lastempty = 1;
			queue->back = queue->arraysize - 1;
		}
		else {
			queue->back--;
		}
		queue->count--;
		if (queue->count == 1) {
			queue->front = queue->back;
		}
		else if (queue->count == 0 && queue->arrays->count == 2) {
			/* Both arrays are empty - remove either one */
			free(dynarray_remove_tail(queue->arrays));
		}
	}
	return data;
}

void * cdeque_get_at(const cdeque *queue, unsigned int index)
{
	void * data = NULL;
	if (index < queue->count) {
		const unsigned int pos = index + queue->front;
		data = ((void**)dynarray_get(queue->arrays,
									 pos / queue->arraysize + queue->firstempty))[pos % queue->arraysize];
	}
	return data;
}

void * cdeque_set_at(cdeque *queue, unsigned int index, void * data)
{
	void * result = NULL;
	if (index == queue->count) {
		cdeque_push_back(queue, data);
	}
	else if (index < queue->count) {
		const unsigned int pos = index + queue->front;
		result = cdeque_get_at(queue, index);
		((void**)dynarray_get(queue->arrays,
							  pos / queue->arraysize + queue->firstempty))[pos % queue->arraysize] = data;
	}
	return result;
}

void * cdeque_peek_front(const cdeque * queue)
{
	void *data = NULL;
	if (queue->count > 0) {
		data = ((void**)dynarray_get(queue->arrays, queue->firstempty))[queue->front];
	}
	return data;
}

void * cdeque_peek_back(const cdeque * queue)
{
	void *data = NULL;
	if (queue->count > 0) {
		data = ((void**)dynarray_get(queue->arrays,
									 dynarray_get_count(queue->arrays) - 1 - queue->lastempty))[queue->back];
	}
	return data;
}

void cdeque_for_each(const cdeque * queue, cdeque_forfn fun)
{
	unsigned int i;
	for (i = 0; i < queue->count; i++) {
		fun(cdeque_get_at(queue, i));
	}
}