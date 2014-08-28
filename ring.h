/**
 * @file ring.h
 * @brief implement a ring buffer using a array,
 *        which can expand itself when it's full.
 *
 *        there are two stages:
 *        stage1, no expand slot
 *        stage2, the ring slot for pop, the expand slot for push
 *
 *        |  ring slot    |
 *        +---+---+---+---+---+---+---+---+
 *        |   |   |   |   |   |   |   |   |
 *        +---+---+---+---+---+---+---+---+
 *                        |  expand slot  |
 *
 *        when the ring slot is full or the expand slot is full,
 *        the whole slot will be expanded. In the first case, the stage,
 *        wile be changed from stage1 to stage2.
 *
 *        when the ring slot is empty and the expand slot is not empty,
 *        the stage will be changed from stage2 to stage1.
 *
 *        note that, the whole slot number can be calculate by r->cap
 *        and r->num2, because we always expand the slot by N power of 2
 * @auther mincore@163.com
 */
#ifndef _RING_H
#define _RING_H

#include <stdlib.h>
#include <string.h>

struct ring {
	void **slot;
	int cap;	/* ring slot cap */
	int num;	/* item number in ring slot */
	int tail;	/* tail index in ring slot */
	int num2;	/* item number in expand slot */
};

#define is_power_of_2(n) ((((n) - 1) & (n)) == 0)

static inline int boundup_power_of_two(int n)
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;

	return n;
}

static inline void ring_init(struct ring *r, int order)
{
	memset(r, 0, sizeof(*r));
	r->cap = 1<<order;
	r->slot = (void**)malloc(sizeof(void*)*r->cap);
}

static inline int ring_need_expand(struct ring *r)
{
	return r->num2 == 0 ? r->num == r->cap :
		is_power_of_2(r->cap + r->num2);
}

static inline void * ring_push(struct ring *r, void *ptr)
{
	if (ring_need_expand(r)) {
		/* stage2 */
		int newcap = (r->cap + r->num2) * 2;
		r->slot = (void**)realloc(r->slot, sizeof(void*)*newcap);
		r->slot[r->cap + r->num2] = ptr;
		r->num2++;
		return ptr;
	}

	if (r->num2 == 0) {
		/* stage1 */
		r->slot[r->tail] = ptr;
		r->tail = (r->tail + 1) % r->cap;
		r->num++;
	} else {
		/* stage2 */
		r->slot[r->cap + r->num2] = ptr;
		r->num2++;
	}

	return ptr;
}

static inline void * ring_pop(struct ring *r)
{
	int pop;

	if (r->num == 0)
		return NULL;

	pop = (r->tail - r->num + r->cap) % r->cap;
	r->num--;

	/* stage2 to stage1 */
	if (r->num == 0 && r->num2 != 0) {
		r->tail = r->cap + r->num2;
		r->num = r->num2;
		r->cap = boundup_power_of_two(r->cap + r->num2);
		r->num2 = 0;
	}

	return r->slot[pop];
}

static inline void ring_destroy(struct ring *r)
{
	free(r->slot);
	memset(r, 0, sizeof(*r));
}

#endif
