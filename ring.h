#ifndef _RING_H
#define _RING_H

#include <stdlib.h>
#include <string.h>

struct ring {
	void **slot;
	int cap;
	int num;
	int tail;
};

static inline void ring_init(struct ring *r, int cap)
{
	memset(r, 0, sizeof(*r));
	r->slot = (void**)malloc(sizeof(void*)*cap);
	r->cap = cap;
}

static inline void * ring_push(struct ring *r, void *ptr)
{
	if (r->num == r->cap)
		return NULL;

	r->slot[r->tail] = ptr;
	r->tail = (r->tail + 1) % r->cap;
	r->num++;
	return ptr;
}

static inline void * ring_pop(struct ring *r)
{
	int pop;

	if (r->num == 0)
		return NULL;

	pop = (r->tail - r->num + r->cap) % r->cap;
	r->num--;

	return r->slot[pop];
}

static inline void ring_destroy(struct ring *r)
{
	free(r->slot);
}

#endif
